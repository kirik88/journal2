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
    if (journals) delete journals;
    delete ui;
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
    if (journals->loader->login(true, login, password))
    {
        Answer *answer = journals->loader->lastAnswer;

        if (answer->getCode() != OK)
        {
            ui->labelLoginError->setText(tr("Ошибка при подключении: ") + answer->getResult());
            ui->labelLoginError->show();
        }
        else
        {
            changeMainMode(mmJournals);
        }
//        // получение классов
//        if (getClasses(true))
//        {
//            // загрузка журналов
//            loadJournals(true);
//        }
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

