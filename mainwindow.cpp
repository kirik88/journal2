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
    //readSettings();

    // установка сигналов
    //connectSignals();

    // выставляем текущий режим работы на "подключение"
    changeMainMode(mmLogin);
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
    //updateWidgets();
}

/*void MainWindow::buttonClick()
{
    Loader *loader = new Loader("journal2.titul2005.ru", "test");

    QObject::connect(loader, SIGNAL(loginFinished(Answer *)), this, SLOT(loginFinished(Answer *)));

    loader->login("admin", "admin");
}

void MainWindow::loginFinished(Answer *answer)
{
    QMessageBox::warning(this, tr("Debug message"), answer->getResult());
}*/

