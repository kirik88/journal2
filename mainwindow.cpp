#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // генератор псевдослучайных чисел
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    // стиль приложения переопределяем только для Windows
    #if defined(Q_OS_WIN)
        QApplication::setStyle(QStyleFactory::create("Fusion"));
        QApplication::setPalette(QApplication::style()->standardPalette());
    #endif

    ui->setupUi(this);

    // подготовить приложение
    prepareApplication();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete treeJournals;
    delete tableJournal;
    delete loading;
    if (journals) delete journals;
    delete glass;
}

// подготовка приложения при первом запуске
void MainWindow::prepareApplication()
{
    // инициализируем внутренние переменные
    commonIni = QString("%1/%2.ini").arg(QApplication::applicationDirPath()).arg(QApplication::applicationName());
    glass = new Glass();
    journals = 0;
    currentJournal = 0;

    // инициализируем надписи
    labelBackText = tr("« Вернуться к журналу");
    labelRefreshText = tr("Обновить");

    // надпись-кнопка "Вернуться к журналу"
    labelBack = new QLabel(QString("<html><a href=\"back\">%1</a></html>").arg(labelBackText));
    labelBack->hide();

    // надпись-кнопка "Обновить"
    labelRefresh = new QLabel(QString("<html><a href=\"journal\">%1</a></html>").arg(labelRefreshText));
    labelRefresh->hide();

    // дерево журналов
    treeJournals = new JournalsTreeWidget(ui->treeJournals->parentWidget());
    delete ui->treeJournals;
    treeJournals->parentWidget()->layout()->addWidget(treeJournals);

    // таблица журнала
    tableJournal = new JournalTableWidget(ui->tableJournal->parentWidget());
    delete ui->tableJournal;
    tableJournal->parentWidget()->layout()->addWidget(tableJournal);

    // панель инструментов
    ui->toolBar->addWidget(ui->buttonJournals);
    ui->toolBar->addSeparator();
    imageArrow = ui->toolBar->addWidget(ui->imageArrow);
    buttonJournal = ui->toolBar->addWidget(ui->buttonJournal);
    buttonSave = ui->toolBar->addWidget(ui->buttonSave);
    buttonTools = ui->toolBar->addWidget(ui->buttonTools);
    buttonRefresh = ui->toolBar->addWidget(ui->buttonRefresh);
    buttonProcessing = ui->toolBar->addWidget(ui->buttonProcessing);
    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->toolBar->addWidget(spacer);
    ui->toolBar->addWidget(ui->buttonAbout);
    ui->toolBar->addWidget(ui->buttonExit);
    delete ui->frameToolBar;

    // анимация при авторизации
    loading = new QMovie(":/animations/loading_min");
    ui->movieLogin->setMovie(loading);

    // скрываем ненужные пока элементы подключения
    ui->labelLoginError->hide();
    ui->movieLogin->hide();
    ui->buttonLoginCancel->hide();

    // будем отлавливать события окна
    this->installEventFilter(this);

    // загрузка настроек
    readSettings();

    // установка сигналов
    connectSignals();

    // выставляем текущий режим работы на "подключение"
    changeMainMode(mmLogin);
}

// отлов событий окна
// используется, чтобы, например, отловить нажатие "горячих клавиш"
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    // изменения размеров окна
    if ((event->type() == QEvent::Show) || (event->type() == QEvent::Resize))
    {
        updateWidgets();
    }

    // все что относится к таблице
    /*if (ui->tableJournal->hasFocus())
    {
        if (event->type() == QEvent::KeyRelease)
        {
            // текущий элемент
            QTableWidgetItem *item = ui->tableJournal->currentItem();
            if (!item) return QMainWindow::eventFilter(obj, event);

            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

            int key = keyEvent->key();
            if (keyEvent->text().toLower() == markNone) key = Qt::Key_Y; // для русской буквы "н" при выставлении метки "не был"
            if (keyEvent->text().toLower() == tr("ы"))  key = Qt::Key_S; // для русской буквы "ы" при сохранении журнала
            if (keyEvent->text().toLower() == tr("к"))  key = Qt::Key_R; // для русской буквы "к" при обновлении журнала

            switch (key)
            {
            // выставление конкретного значения
            case Qt::Key_0:
                changeValue(item->column(), item->row(), 0);
                break;
            case Qt::Key_1:
                changeValue(item->column(), item->row(), 1);
                break;
            case Qt::Key_2:
                changeValue(item->column(), item->row(), 2);
                break;
            case Qt::Key_3:
                changeValue(item->column(), item->row(), 3);
                break;
            case Qt::Key_4:
                changeValue(item->column(), item->row(), 4);
                break;
            case Qt::Key_5:
                changeValue(item->column(), item->row(), 5);
                break;

            // выставление метки "не был"
            case Qt::Key_Y:
                changeValue(item->column(), item->row(), markNone);
                break;

            // изменение значения
            case Qt::Key_Space:
                if (user && journal && (user->readonly || journal->readonly))
                {
                    showValueComments(item->column(), item->row());
                }
                else
                {
                    if (keyEvent->modifiers() & Qt::CTRL)
                        nextValue(item->column(), item->row()); // быстрая смена значения
                    else
                        changeValue(item->column(), item->row()); // обычное изменение
                }
                break;

            // очистка значения
            case Qt::Key_Backspace:
            case Qt::Key_Delete:
            case Qt::Key_Period:
                clearValue(item->column(), item->row());
                break;

            // сохранение журнала
            case Qt::Key_S:
                // проверка, что зажат Ctrl
                if (keyEvent->modifiers() != Qt::ControlModifier) return QMainWindow::eventFilter(obj, event);

                // защита от дурака
                if (!user || user->readonly || !journal || journal->readonly) return QMainWindow::eventFilter(obj, event);

                on_buttonSave_clicked();
                break;

            // обновление журнала
            case Qt::Key_R:
                // проверка, что зажат Ctrl
                if (keyEvent->modifiers() != Qt::ControlModifier) return QMainWindow::eventFilter(obj, event);

                on_buttonRefresh_clicked();
                break;
            }
        }
    }*/

    return QMainWindow::eventFilter(obj, event);
}

// процедура чтения настроек
void MainWindow::readSettings()
{
    // вспомогательные переменные
    QString login = "";
    QString password = "";
    QString site = "";
    QString storage = "";

    /* настройки пользователя */
    QSettings *settings = new QSettings();

    // состояние главного окна
    settings->beginGroup("mainwindow");
    this->resize(settings->value("size", QSize(900, 550)).toSize());
    if (settings->contains("pos")) this->move(settings->value("pos").toPoint());
    if (settings->value("maximized", false).toBool()) this->showMaximized();
    settings->endGroup();

    // данные хранилища
    settings->beginGroup("server");
    site    = settings->value("http"   ).toString();
    storage = settings->value("storage").toString();
    settings->endGroup();

    // данные авторизации (в том числе пароль, если сохранён)
    settings->beginGroup("login");
    if (settings->contains("login"))
    {
        login = settings->value("login").toString();
        password = settings->value("password").toString();
    }
    settings->endGroup();

    // данные прокси-сервера
    /*settings->beginGroup("proxy");
    useProxy      =   settings->value("use",      false).toBool();
    proxy.setHostName(settings->value("hostName", "").toString());
    proxy.setPort    (settings->value("port",      0).toInt());
    proxy.setUser    (settings->value("user",     "").toString());
    proxy.setPassword(settings->value("password", "").toString());
    proxySavePwd  =   settings->contains("user");
    settings->endGroup();*/

    // другие настройки
    settings->beginGroup("options");
    useDialogGlass = settings->value("dialogGlass", true).toBool();
    settings->endGroup();

    delete settings;

    /* если в настройках пользователя нет информации о сервере, эта информация грузится из общих настроек */
    if (site == "" || storage == "")
    {
        settings = new QSettings(commonIni, QSettings::IniFormat);

        // сервер с журналами
        settings->beginGroup("server");
        if (site    == "") site    = settings->value("http"   ).toString();
        if (storage == "") storage = settings->value("storage").toString();
        settings->endGroup();

        delete settings;
    }

    // фиксируем настройки
    if (site != "" && storage != "")
    {
        Loader *loader = new Loader(site, storage);
        journals = new Journals(loader);

        ui->editLogin->setText(login);
        ui->editPassword->setText(password);
    }
}

// процедура записи настроек
void MainWindow::writeSettings(Setting setting)
{
    /* настройки пользователя */
    QSettings *settings = new QSettings();

    // состояние главного окна
    if (setting == sAll || setting == sWindow)
    {
        settings->beginGroup("mainwindow");
        settings->setValue("maximized", this->isMaximized());
        if (!this->isMaximized())
        {
            settings->setValue("size", this->size());
            settings->setValue("pos",  this->pos());
        }
        settings->endGroup();
    }

    // данные хранилища
    if (setting == sAll || setting == sStorage)
    {
        if (journals && journals->loader)
        {
            settings->beginGroup("server");
            settings->setValue("http", journals->loader->site);
            settings->setValue("storage", journals->loader->storage);
            settings->endGroup();
        }
    }

    // данные авторизации
    if (setting == sAll || setting == sLogin)
    {
        if (journals && journals->loader && journals->loader->user)
        {
            settings->beginGroup("login");
            settings->setValue("login", journals->loader->user->login);
            //settings->setValue("password", journals->loader->user->password);
            settings->endGroup();
        }
    }

    // данные прокси-сервера
    if (setting == sAll || setting == sProxy)
    {
        /*settings->beginGroup("proxy");
        settings->setValue("use", useProxy);
        settings->setValue("hostName", proxy.hostName());
        settings->setValue("port", proxy.port());
        if (proxySavePwd)
        {
            settings->setValue("user", proxy.user());
            settings->setValue("password", proxy.password());
        }
        else
        {
            settings->remove("user");
            settings->remove("password");
        }
        settings->endGroup();*/
    }

    delete settings;
}

// установка сигналов
void MainWindow::connectSignals()
{
    /* общее */
    QObject::connect(labelBack, SIGNAL(linkActivated(const QString &)),
            this, SLOT(labelBack_clicked()));
    QObject::connect(labelRefresh, SIGNAL(linkActivated(const QString &)),
            this, SLOT(labelRefresh_clicked()));

    /* подключение к системе */
    QObject::connect(ui->buttonLoginExit, SIGNAL(clicked()),
            this, SLOT(on_buttonExit_clicked()));
    QObject::connect(ui->editLogin, SIGNAL(returnPressed()),
            this, SLOT(on_buttonLogin_clicked()));
    QObject::connect(ui->editPassword, SIGNAL(returnPressed()),
            this, SLOT(on_buttonLogin_clicked()));

    /* дерево журналов */
    QObject::connect(treeJournals, SIGNAL(createJournal()),
            this, SLOT(treeJournals_createJournal()));
    QObject::connect(treeJournals, SIGNAL(openJournal(int)),
            this, SLOT(treeJournals_openJournal(int)));
    QObject::connect(treeJournals, SIGNAL(editJournal(int)),
            this, SLOT(treeJournals_editJournal(int)));
    QObject::connect(treeJournals, SIGNAL(deleteJournal(int)),
            this, SLOT(treeJournals_deleteJournal(int)));

    /* таблица */
    QObject::connect(tableJournal, SIGNAL(journalChanged()),
            this, SLOT(tableJournal_journalChanged()));

//    // говорим, что у заголовков тоже есть контекстное меню
//    tableJournal->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
//    tableJournal->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

//    /* таблица */
//    // колонки
//    QObject::connect(tableJournal->horizontalHeader(), SIGNAL(sectionClicked(int)),
//            this, SLOT(tableJournal_columnClicked(int)));
//    QObject::connect(tableJournal->horizontalHeader(), SIGNAL(customContextMenuRequested(const QPoint &)),
//            this, SLOT(tableJournal_columnContext(const QPoint &)));
//    // записи
//    QObject::connect(tableJournal->verticalHeader(), SIGNAL(sectionClicked(int)),
//            this, SLOT(tableJournal_rowClicked(int)));
//    QObject::connect(tableJournal->verticalHeader(), SIGNAL(customContextMenuRequested(const QPoint &)),
//            this, SLOT(tableJournal_rowContext(const QPoint &)));
//    // значения
//    QObject::connect(tableJournal, SIGNAL(customContextMenuRequested(const QPoint &)),
//            this, SLOT(tableJournal_showContextMenu()));
}

// показ/скрытие кнопок
void MainWindow::checkButtons()
{
    bool allowEditJournal = (journals && Security::allowEditJournal(journals->loader->user, currentJournal));

    // кнопки на панели инструментов
    buttonJournal->setVisible(currentJournal);
    buttonSave->setVisible(allowEditJournal);
    buttonTools->setVisible(false);//buttonSave->isVisible());
    buttonRefresh->setVisible(currentJournal);
    buttonProcessing->setVisible(false);//!user->readonly && !journal->readonly);

    // иконка журнала
    if (currentJournal && currentJournal->isNew) ui->buttonJournal->setIcon(QIcon(":/icons/new"));
    else if (currentJournal && currentJournal->isDeleted) ui->buttonJournal->setIcon(QIcon(":/icons/delete"));
    else if (currentJournal && currentJournal->isArchived) ui->buttonJournal->setIcon(QIcon(":/icons/archive"));
    //else if (allowEditJournal) ui->buttonJournal->setIcon(QIcon(":/icons/journal_write"));
    else ui->buttonJournal->setIcon(QIcon(":/icons/journal"));

    // декоративная стрелка
    imageArrow->setVisible(buttonJournal->isVisible());

    // наименование журнала
    if (currentJournal) ui->buttonJournal->setText(currentJournal->getName());
}

// смена режима приложения
void MainWindow::changeMainMode(MainMode mode)
{
    checkButtons();

    switch (mode)
    {
    // подключение к системе
    case mmLogin:
        ui->toolBar->hide();
        ui->frameJournals->hide();
        ui->frameJournal->hide();
        ui->frameTools->hide();

        ui->frameLogin->show();

        // если не загружены настройки сервера
        if (!journals)
        {
            ui->labelLoginError->setText(tr("Не настроен сервер электронного журнала."));
            ui->labelLoginError->show();

            ui->editLogin->setEnabled(false);
            ui->editPassword->setEnabled(false);
            ui->buttonLogin->setEnabled(false);

            ui->buttonLoginExit->setFocus();
        }
        else
        {
            if (ui->editLogin->text() == "") ui->editLogin->setFocus();
            else ui->editPassword->setFocus();
        }

        break;

    // список доступных журналов
    case mmJournals:
        ui->frameLogin->hide();
        ui->frameJournal->hide();
        ui->frameTools->hide();

        ui->toolBar->show();
        ui->frameJournals->show();

        ui->buttonJournals->setCheckable(true);
        ui->buttonJournals->setChecked(true);

        ui->buttonJournal->setChecked(false);
        ui->buttonJournal->setCheckable(false);

        ui->buttonTools->setChecked(false);
        ui->buttonTools->setCheckable(false);

        labelBack->setParent(treeJournals);
        labelBack->setVisible(currentJournal);

        labelRefresh->setParent(treeJournals);
        labelRefresh->show();

        treeJournals->setFocus();

        break;

    // текущий журнал
    case mmJournal:
        ui->frameLogin->hide();
        ui->frameJournals->hide();
        ui->frameTools->hide();

        ui->toolBar->show();
        ui->frameJournal->show();

        ui->buttonJournals->setChecked(false);
        ui->buttonJournals->setCheckable(false);

        ui->buttonJournal->setCheckable(true);
        ui->buttonJournal->setChecked(true);

        ui->buttonTools->setChecked(false);
        ui->buttonTools->setCheckable(false);

        tableJournal->setFocus();

        break;

    // инструменты
    case mmTools:
        ui->frameLogin->hide();
        ui->frameJournals->hide();
        ui->frameJournal->hide();

        // заполнить список инструментов
        //fillTools();

        ui->frameTools->show();

        ui->buttonJournals->setChecked(false);
        ui->buttonJournals->setCheckable(false);

        ui->buttonJournal->setChecked(false);
        ui->buttonJournal->setCheckable(false);

        ui->buttonTools->setCheckable(true);
        ui->buttonTools->setChecked(true);

        labelBack->setParent(ui->listTools);
        labelBack->setVisible(currentJournal);

        ui->listTools->setFocus();
    }

    // запоминание текущего режима
    currentMode = mode;

    // обновление заголовка окна
    updateWindowTitle();

    // обновление виджетов
    updateWidgets();
}

// обновление размеров и положений виджетов
void MainWindow::updateWidgets()
{
    // надпись-ссылка "Вернуться к журналу"
    if (labelBack->parentWidget())
        labelBack->move(labelBack->height(),
                labelBack->parentWidget()->contentsRect().height() - labelBack->height() * 1.5);

    // надпись-ссылка "Обновить"
    if (labelRefresh->parentWidget())
        labelRefresh->move(labelRefresh->parentWidget()->contentsRect().width() - labelRefresh->width() - labelRefresh->height(),
                labelRefresh->parentWidget()->contentsRect().height() - labelRefresh->height() * 1.5);
}

// обновление заголовка окна
void MainWindow::updateWindowTitle()
{
    QString title = tr("Мой журнал 2");

    if (currentJournal)
    {
        title = tr("%1[*] - %2").arg(currentJournal->getName().replace("\n", " ")).arg(title);
    }

    if (journals && journals->loader->user)
    {
        title = tr("%1 - %2").arg(title).arg(journals->loader->user->name);
        //if (user->readonly) title = tr("%1 [только просмотр]").arg(title);
    }

    this->setWindowTitle(title);

    if (currentJournal) this->setWindowModified(currentJournal->isChanged);
}

// реакция на закрытие приложения
void MainWindow::closeEvent(QCloseEvent *event)
{
    // запрос на сохранение
    if (currentJournal && !checkSaveJournal(tr("Сохранить изменения в журнале «%1» перед закрытием?").arg(currentJournal->getName())))
    {
        event->ignore();
    }

    // прерываем все операции
    if (journals) journals->abort();

    // сохраняем состояние окна
    writeSettings(sWindow);
}

// проверка, сохранялся ли журнал в выводом сообщения и автоматическим сохранением
bool MainWindow::checkSaveJournal(const QString &text, bool allowSave)
{
    if (currentJournal && currentJournal->isChanged)
    {
        QMessageBox *question = new QMessageBox(QMessageBox::Question, tr("Журнал не сохранён"), text);

        // кнопка "Отмена"
        QPushButton *btn = new QPushButton(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton), tr("Отмена"));
        btn->setObjectName("checkCancel");
        question->addButton(btn, QMessageBox::RejectRole);

        // кнопка "Не сохранять"
        btn = new QPushButton(QApplication::style()->standardIcon(QStyle::SP_DialogDiscardButton), tr("Не сохранять"));
        btn->setObjectName("checkDiscard");
        question->addButton(btn, QMessageBox::DestructiveRole);
        //connect(btn, SIGNAL(clicked()), this, SLOT(buttonSaveJournalDialog_clicked()));

        if (allowSave) // при allowSave должна быть возможность сохранить журнал
        {
            // кнопка "Сохранить"
            btn = new QPushButton(QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton), tr("Сохранить"));
            btn->setObjectName("checkSave");
            question->addButton(btn, QMessageBox::YesRole);
            //connect(btn, SIGNAL(clicked()), this, SLOT(buttonSaveJournalDialog_clicked()));
        }

        // устанавливаем затемнение
        if (useDialogGlass) glass->install(this);

        // показываем диалоговое окно
        int result = question->exec();

        // убираем затемнение
        if (useDialogGlass) glass->remove();

        switch (result)
        {
        case 1: // нажали "Не сохранять"
            return true;

        case 2: // нажали "Сохранить"
            // сохраняем журнал
            currentJournal->isChanged = false;
            currentJournal->isNew = false;

            return true;

        default: // нажали "Отмена"
            return false;
        }
    }

    return true;
}

// реакция на нажатие кнопки "Войти в систему"
void MainWindow::on_buttonLogin_clicked()
{
    // вспомогательные переменные
    QString login = ui->editLogin->text();
    QString password = ui->editPassword->text();

    // должен быть введён хотя бы логин
    if (login == "")
    {
        ui->labelLoginError->setText(tr("Необходимо ввести регистрационные данные."));
        ui->labelLoginError->show();

        ui->editLogin->setFocus();

        return;
    }

    // работа с прокси сервером
    //updateProxy();

    // надпись об ошибках
    ui->labelLoginError->hide();

    // смена кнопок
    ui->buttonLogin->hide();
    ui->buttonLoginCancel->setEnabled(true);
    ui->buttonLoginCancel->show();
    ui->buttonLoginCancel->setFocus();

    // блокировка кнопок
    ui->frameLoginEdits->setEnabled(false);

    // анимация
    ui->movieLogin->movie()->start();
    ui->movieLogin->show();

    QApplication::processEvents();

    // подключение
    QString message;
    if (
            journals->tryLogin(login, password, &message) && // подключаемся
            journals->refreshJournals(&message) && // грузим список журналов
            journals->refreshData(&message) // грузим список классов, предметов и учителей
        )
    {
        // строим дерево журналов
        treeJournals->setJournals(journals);

        // меняем страницуr
        changeMainMode(mmJournals);

        // сохраняем настройки авторизации
        writeSettings(sLogin);
    }
    else
    {
        // если сообщения нет - операцию отменили
        if (message != "")
        {
            ui->labelLoginError->setText(tr("Ошибка при подключении: ") + message);
            ui->labelLoginError->show();
        }
    }

    // снова отображаем кнопки
    ui->buttonLogin->show();
    ui->buttonLoginCancel->hide();
    ui->buttonLogin->setFocus();

    // останавливаем анимацию
    ui->movieLogin->movie()->stop();
    ui->movieLogin->hide();

    // разблокировка кнопок
    ui->frameLoginEdits->setEnabled(true);

    // переключение фокуса
    ui->editPassword->clear();
    ui->editPassword->setFocus();
}

// реакция на нажатие кнопки "Отмена" при подключении к системе
void MainWindow::on_buttonLoginCancel_clicked()
{
    ui->buttonLoginCancel->setEnabled(false);

    journals->abort();
}

// реакция на нажатие кнопки "Журналы"
void MainWindow::on_buttonJournals_clicked()
{
    if (ui->buttonJournals->isCheckable() && currentJournal)
        changeMainMode(mmJournal);
    else
        changeMainMode(mmJournals);
}

// реакция на нажатие кнопки с наименованием журнала
void MainWindow::on_buttonJournal_clicked()
{
    // переключение страницы
    changeMainMode(mmJournal);
}

// реакция на нажатие кнопки "Сохранить"
void MainWindow::on_buttonSave_clicked()
{
    // затемняем окно
    glass->install(this, tr("Сохранение журнала ..."));

    // сохраняем журнал
    QString message;
    if (journals->saveJournal(currentJournal, &message))
    {
        // помечаем, что журнал не изменялся
        currentJournal->isChanged = false;
        currentJournal->isNew = false;

        // перестраиваем дерево журналов
        treeJournals->setJournals(journals);
    }

    // убираем затемнение в случае, если оно не требуется для диалоговых окон
    if (useDialogGlass) glass->hide();
    else glass->remove();

    if (message != "")
    {
        QMessageBox::critical(this, tr("Сохранение журнала"),
            tr("Ошибка при сохранении журнала:\n%1").arg(message));
    }

    // убираем затемнение
    glass->remove();
}

// реакция на нажатие кнопки "Обновить"
void MainWindow::on_buttonRefresh_clicked()
{
    if (!currentJournal || !checkSaveJournal(tr("Обновление журнала приведёт к потере изменений. Вы уверены?"), false))
        return;

    // затемняем окно
    glass->install(this, tr("Загрузка журнала ..."));

    // загружаем журнал
    QString message;
    Journal *loaded = 0;
    if (journals->getJournal(currentJournal->getId(), loaded, &message))
    {
        // работаем с копией загруженного журнала
        if (currentJournal) delete currentJournal;
        currentJournal = new Journal(loaded);

        // помечаем, что журнал не изменялся
        currentJournal->isChanged = false;

        // выводим данные журнала
        tableJournal->setJournal(currentJournal);

        // перестраиваем дерево журналов
        treeJournals->setJournals(journals);

        // переключаем страницу
        changeMainMode(mmJournal);
    }

    // убираем затемнение в случае, если оно не требуется для диалоговых окон
    if (useDialogGlass) glass->hide();
    else glass->remove();

    if (message != "")
    {
        QMessageBox::critical(this, tr("Загрузка журнала"),
            tr("Ошибка при загрузке журнала:\n%1").arg(message));
    }

    // убираем затемнение
    glass->remove();
}

// реакция на нажатие кнопки "О программе"
void MainWindow::on_buttonAbout_clicked()
{
    // установка затемнения
    if (useDialogGlass) glass->install(this);

    QMessageBox::about(this, tr("О программе"),
         tr("<html><b>Электронный журнал «Мой журнал 2»</b> версии %1"
            "<p>Программа разработана в Qt под лицензией LGPL и является разработкой с открытым исходным кодом; "
            "вы можете использовать программу в любых целях, не забывая имени её автора.</p>"
            "<p>Вы также можете помочь в разработке, отправив письмо с "
            "пожеланиями и рекомендациями на <a href=\"mailto:maloletkovki@gmail.com\">maloletkovki@gmail.com</a>.</p>"
            "<i>Кирилл Малолетков, Саратов, 2013 г.</i></html>").arg(appVersion));

    // убираем затемение
    glass->remove();
}

// реакция на нажатие кнопки "Выйти"
void MainWindow::on_buttonExit_clicked()
{
    this->close();
}

// событие при создании нового журнала
void MainWindow::treeJournals_createJournal()
{
    // диалоговое окно нового журнала
    JournalDialog *dialog = new JournalDialog(journals);
    dialog->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

    // проверяем на нажатие
    bool ok = false;
    while (!ok)
    {
        // затемняем окно
        if (useDialogGlass) glass->install(this);

        // отображаем окно
        if (dialog->exec() == QDialog::Accepted)
        {
            // затемняем окно
            glass->install(this, tr("Создание нового журнала ..."));

            // создаём журнал
            Journal *created = new Journal();
            created->name = dialog->name;
            created->classId = dialog->classId;
            created->courseId = dialog->courseId;
            created->teacherId = dialog->teacherId;
            created->description = dialog->description;
            created->isAuto = true;

            // сохраняем журнал как новый
            QString message;
            if (journals->createJournal(created, &message))
            {
                glass->install(this, tr("Обновление дерева журналов ..."));

                QString message;
                if (journals->refreshJournals(&message))
                {
                    // перестраиваем дерево журналов
                    treeJournals->setJournals(journals);

                    // создание прошло успешно
                    ok = true;
                }
            }

            // убираем затемнение в случае, если оно не требуется для диалоговых окон
            if (useDialogGlass) glass->hide();
            else glass->remove();

            if (!ok && message != "")
            {
                QMessageBox::critical(this, tr("Создание журнала"),
                    tr("Ошибка при создании журнала:\n%1").arg(message));
            }

            delete created;
        }
        else
        {
            // отменили
            ok = true;
        }

        // убираем затемнение
        if (!useDialogGlass) glass->remove();
    }

    // убираем затемнение
    glass->remove();
}

// событие при открытии журнала
void MainWindow::treeJournals_openJournal(int id)
{
    if (currentJournal && !checkSaveJournal(tr("Сохранить изменения в журнале «%1» перед открытием выбранного?").arg(currentJournal->getName())))
        return;

    // затемняем окно
    glass->install(this, tr("Загрузка журнала ..."));

    // загружаем журнал
    QString message;
    Journal *loaded = 0;
    if (journals->getJournal(id, loaded, &message))
    {
        // работаем с копией загруженного журнала
        if (currentJournal) delete currentJournal;
        currentJournal = new Journal(loaded);

        // помечаем, что журнал не изменялся
        currentJournal->isChanged = false;

        // выводим данные журнала
        tableJournal->setJournal(currentJournal);

        // переключение страницы
        changeMainMode(mmJournal);
    }

    // убираем затемнение в случае, если оно не требуется для диалоговых окон
    if (useDialogGlass) glass->hide();
    else glass->remove();

    if (message != "")
    {
        QMessageBox::critical(this, tr("Загрузка журнала"),
            tr("Ошибка при загрузке журнала:\n%1").arg(message));
    }

    // убираем затемнение
    glass->remove();
}

// событие при редактировании журнала
void MainWindow::treeJournals_editJournal(int id)
{

}

// событие при удалении журнала
void MainWindow::treeJournals_deleteJournal(int id)
{
    // получаем журнал, который требуется удалить
    Journal *journal;
    if (!journals->getJournal(id, journal, 0, false)) return;

    // подтверждение
    QMessageBox *question = new QMessageBox(QMessageBox::Question, tr("Удаление журнала"),
                                            tr("Вы действительно хотите удалить журнал «%1»%2?").arg(journal->getName()).arg(journal->isDeleted ? " без возможности восстановления" : ""));

    // кнопка "Отмена"
    QPushButton *btn = new QPushButton(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton), tr("Отмена"));
    question->addButton(btn, QMessageBox::RejectRole);

    // кнопка "Удалить"
    btn = new QPushButton(QApplication::style()->standardIcon(QStyle::SP_DialogDiscardButton), tr("Удалить%1").arg(journal->isDeleted ? " навсегда" : ""));
    question->addButton(btn, QMessageBox::AcceptRole);

    if (journal && !checkSaveJournal(tr("Вы действительно хотите удалить журнал «%1»?").arg(journal->getName())))
        return;

    // установка затемнения
    if (useDialogGlass) glass->install(this);

    // подтвердили
    if (question->exec() == QMessageBox::Accepted)
    {
        // затемняем окно
        glass->install(this, tr("Удаление журнала ..."));

        // удаляем журнал
        QString message;
        bool deleted = (journal->isDeleted ? journals->eraseJournal(id, &message) : journals->deleteJournal(id, &message));

        // обновляем дерево журналов
        if (deleted && journals->refreshJournals(&message))
        {
            // перестраиваем дерево журналов
            treeJournals->setJournals(journals);

            // если удаляли текущий активный журнал - закрываем его
            if (currentJournal && (currentJournal->getId() == id))
            {
                currentJournal = 0;
                tableJournal->setJournal(0);

                // переходим на страницу с журналами
                changeMainMode(mmJournals);
            }
        }

        // убираем затемнение в случае, если оно не требуется для диалоговых окон
        if (useDialogGlass) glass->hide();
        else glass->remove();

        if (message != "")
        {
            QMessageBox::critical(this, tr("Удаление журнала"),
                tr("Ошибка при удалении журнала:\n%1").arg(message));
        }
    }

    // убираем затемнение
    glass->remove();
}

// реакция на изменение данных журнала внутри виджета с таблицей
void MainWindow::tableJournal_journalChanged()
{
    updateWindowTitle();
}

// реакция на нажатие надписи-ссылки "Вернуться к журналу"
void MainWindow::labelBack_clicked()
{
    // защита от дурака
    if (!currentJournal) return;

    // переключаемся на журнал
    changeMainMode(mmJournal);
}

// реакция на нажатие надписи-ссылки "Обновить"
void MainWindow::labelRefresh_clicked()
{
    if (currentMode == mmJournals)
    {
        glass->install(this, tr("Обновление дерева журналов ..."));

        QString message;
        if (journals->refreshJournals(&message))
        {
            // перестраиваем дерево журналов
            treeJournals->setJournals(journals);
        }
        else
        {
            // убираем затемнение в случае, если оно не требуется для диалоговых окон
            if (useDialogGlass) glass->hide();
            else glass->remove();

            // если сообщения нет - операцию отменили
            if (message != "")
            {
                ui->labelLoginError->setText(tr("Ошибка при получении журналов: ") + message);
                ui->labelLoginError->show();
            }
        }

        glass->remove();
    }
}
