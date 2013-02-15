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
    if (journals) delete journals;
}

// подготовка приложения при первом запуске
void MainWindow::prepareApplication()
{
    // инициализируем внутренних переменных
    commonIni = QString("%1/%2.ini").arg(QApplication::applicationDirPath()).arg(QApplication::applicationName());
    //glass = new Glass();
    journals = 0;

    // инициализируем надписей
    labelBackText = tr("« Вернуться к журналу");
    labelRefreshText = tr("Обновить");
    markNone = tr("н");

    // надпись-кнопка "Вернуться к журналу"
    labelBack = new QLabel(QString("<html><a href=\"back\">%1</a></html>").arg(labelBackText));
    labelBack->hide();

    // надпись-кнопка "Обновить"
    labelRefresh = new QLabel(QString("<html><a href=\"journal\">%1</a></html>").arg(labelRefreshText));
    labelRefresh->hide();

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

    // скрываем ненужные пока элементы подключения
    ui->labelLoginError->hide();
    ui->movieLogin->hide();
    ui->buttonLoginCancel->hide();

    // запрещаем изменение колонок дерева журналов
    ui->treeJournals->header()->setSectionsMovable(false);

    // будем отлавливать события окна
    //this->installEventFilter(this);

    // загрузка настроек
    readSettings();

    // установка сигналов
    connectSignals();

    // выставляем текущий режим работы на "подключение"
    changeMainMode(mmLogin);
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

    // имя пользователя и пароль (если сохранён)
    settings->beginGroup("login");
    if (settings->contains("login"))
    {
        login = settings->value("login").toString();
        password = settings->value("password").toString();
    }
    settings->endGroup();

    // прокси-сервер
    settings->beginGroup("proxy");
    /*useProxy      =   settings->value("use",      false).toBool();
    proxy.setHostName(settings->value("hostName", "").toString());
    proxy.setPort    (settings->value("port",      0).toInt());
    proxy.setUser    (settings->value("user",     "").toString());
    proxy.setPassword(settings->value("password", "").toString());
    proxySavePwd  =   settings->contains("user");*/
    settings->endGroup();

    // сервер с журналами
    settings->beginGroup("server");
    site    = settings->value("http"   ).toString();
    storage = settings->value("storage").toString();
    settings->endGroup();

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

// установка сигналов
void MainWindow::connectSignals()
{
//    /* общее */
//    connect(labelBack, SIGNAL(linkActivated(const QString &)),
//            this, SLOT(labelBack_clicked()));
//    connect(labelRefresh, SIGNAL(linkActivated(const QString &)),
//            this, SLOT(labelRefresh_clicked()));

    /* подключение к системе */
    connect(ui->buttonLoginExit, SIGNAL(clicked()),
            this, SLOT(on_buttonExit_clicked()));
    connect(ui->editPassword, SIGNAL(returnPressed()),
            this, SLOT(on_buttonLogin_clicked()));

//    // говорим, что у заголовков тоже есть контекстное меню
//    ui->tableJournal->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
//    ui->tableJournal->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

//    /* таблица */
//    // колонки
//    connect(ui->tableJournal->horizontalHeader(), SIGNAL(sectionClicked(int)),
//            this, SLOT(tableJournal_columnClicked(int)));
//    connect(ui->tableJournal->horizontalHeader(), SIGNAL(customContextMenuRequested(const QPoint &)),
//            this, SLOT(tableJournal_columnContext(const QPoint &)));
//    // записи
//    connect(ui->tableJournal->verticalHeader(), SIGNAL(sectionClicked(int)),
//            this, SLOT(tableJournal_rowClicked(int)));
//    connect(ui->tableJournal->verticalHeader(), SIGNAL(customContextMenuRequested(const QPoint &)),
//            this, SLOT(tableJournal_rowContext(const QPoint &)));
//    // значения
//    connect(ui->tableJournal, SIGNAL(customContextMenuRequested(const QPoint &)),
//            this, SLOT(tableJournal_showContextMenu()));
}

// смена режима приложения
void MainWindow::changeMainMode(MainMode mode)
{
    //checkButtons();

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
            ui->editPassword->setFocus();
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

        labelBack->setParent(ui->treeJournals);
        //labelBack->setVisible(journal);

        labelRefresh->setParent(ui->treeJournals);
        labelRefresh->show();

        ui->treeJournals->setFocus();

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

        ui->tableJournal->setFocus();

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
        //labelBack->setVisible(journal);

        ui->listTools->setFocus();
    }

    // запоминание текущего режима
    currentMode = mode;

    // обновление заголовка окна
    //updateWindowTitle();

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

// заполнение дерева журналов
void MainWindow::fillTree()
{
    // очищаем
    ui->treeJournals->clear();

    // защита от дурака
    if (!journals || !journals->journals) return;

    for (int i = 0; journals->journals->count() > i; i++)
    {
        Journal *journal = journals->journals->at(i);

        // в зависимости от прав на запись разная иконка
        //QIcon icon(journal->deleted ? ":/icons/delete" : (journal->archived ? ":/icons/journal_archived" :
        //        (!user || user->readonly || !journal || journal->readonly ? ":/icons/journal" : ":/icons/journal_write")
        //        ));
        QIcon icon(":/icons/journal_write");

        QTreeWidgetItem *item = new QTreeWidgetItem(
                QStringList()
                <<  tr("%1").arg(journal->name)
                << ""//(journal->classId > 0 ? classes->classById(journal->classId)->name : "")
                << ""//(journal->year > 0 ? tr("%1-%2").arg(journal->year).arg(journal->year+1) : "")
                <<  journal->description);

        ui->treeJournals->addTopLevelItem(item);

        // название журнала жирное и с иконкой
        QFont font = item->font(0);
        font.setBold(true);
        item->setFont(0, font);
        item->setIcon(0, icon);

        // описание курсивом
        font = item->font(3);
        font.setItalic(true);
        item->setFont(3, font);

        // дата и время изменения справа
        // кнопка на открытие журнала справа и с иконкой
        // реализовано через фрэйм, что даёт:
        // отступ внутри ячейки, выравнивание по правому краю
        QFrame *frame = new QFrame();
        frame->setLayout(new QHBoxLayout);
        frame->setFrameShape(QFrame::NoFrame);

        QWidget *spacer = new QWidget();
        spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        frame->layout()->addWidget(spacer);

        QLabel *label = new QLabel(journal->changed.toString(tr("d MMM yyyy г., HH:mm")));
        frame->layout()->addWidget(label);

        QToolButton *btn = new QToolButton();
        frame->layout()->addWidget(btn);

        btn->setText(tr("открыть"));
        btn->setObjectName(tr("%1%2").arg(buttonOpenName).arg(journal->getId()));
        //btn->setMaximumWidth(76);
        //btn->setLayoutDirection(Qt::RightToLeft);
        btn->setIcon(QIcon(":/icons/16/open"));
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        btn->setCursor(Qt::PointingHandCursor);

        connect(btn, SIGNAL(clicked()), this, SLOT(buttonOpen_clicked()));

        item->setData(0, Qt::UserRole, journal->getId());

        ui->treeJournals->setItemWidget(item, 4, frame);
    }

    // добавление строки "Новый журнал"
    //if (!user->readonly)
    //{
        // иконка
        QIcon icon(":/icons/new");

        QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << tr("Новый журнал"));

        ui->treeJournals->addTopLevelItem(item);

        // название
        QFont font = item->font(0);
        font.setBold(true);
        font.setUnderline(true);
        item->setFont(0, font);
        item->setIcon(0, icon);

        // описание
        font = item->font(3);
        font.setItalic(true);
        item->setFont(3, font);

        // кнопка
        QFrame *frame = new QFrame();
        frame->setLayout(new QHBoxLayout);
        frame->setFrameShape(QFrame::NoFrame);

        QWidget *spacer = new QWidget();
        spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        frame->layout()->addWidget(spacer);

        QToolButton *btn = new QToolButton();
        frame->layout()->addWidget(btn);

        btn->setText(tr("создать"));
        btn->setObjectName(tr("%1%2").arg(buttonOpenName).arg(0));
        //btn->setLayoutDirection(Qt::RightToLeft);
        btn->setIcon(QIcon(":/icons/16/plus"));
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        btn->setCursor(Qt::PointingHandCursor);

        connect(btn, SIGNAL(clicked()), this, SLOT(buttonOpen_clicked()));

        item->setData(0, Qt::UserRole, 0);

        ui->treeJournals->setItemWidget(item, 4, frame);
    //}

    // выравнивание по содержимому
    for (int j = 0; j < ui->treeJournals->header()->count(); j++)
    {
        ui->treeJournals->resizeColumnToContents(j);
    }
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

    QApplication::processEvents();

    // анимация
    QMovie *movie = new QMovie(":/animations/loading_min");
    ui->movieLogin->setMovie(movie);
    movie->start();
    ui->movieLogin->show();

    // подключение
    QString message;
    if (journals->tryLogin(login, password, &message))
    {
        if (journals->refreshJournals(&message))
        {
            // строим дерево журналов
            fillTree();

            // меняем страницу
            changeMainMode(mmJournals);
        }
        else
        {
            ui->labelLoginError->setText(tr("Ошибка при получении журналов: ") + message);
            ui->labelLoginError->show();
        }
    }
    else
    {
        ui->labelLoginError->setText(tr("Ошибка при подключении: ") + message);
        ui->labelLoginError->show();
    }

    // снова отображаем кнопки
    ui->buttonLogin->show();
    ui->buttonLoginCancel->hide();
    ui->buttonLogin->setFocus();

    // останавливаем анимацию
    ui->movieLogin->movie()->stop();
    ui->movieLogin->hide();
    ui->movieLogin->setMovie(0);
    delete movie;

    // разблокировка кнопок
    ui->frameLoginEdits->setEnabled(true);

    // переключение фокуса
    ui->editPassword->clear();
    ui->editPassword->setFocus();
}

// реакция на нажатие кнопки "Выйти"
void MainWindow::on_buttonExit_clicked()
{
    this->close();
}

/*void MainWindow::loginFinished(Answer *answer)
{
    QMessageBox::warning(this, tr("Debug message"), answer->getResult());
}*/

