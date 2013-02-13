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

    //QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(buttonClick()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buttonClick()
{
    Loader *loader = new Loader("journal2.titul2005.ru", "test");

    QObject::connect(loader, SIGNAL(loginFinished(Answer *)), this, SLOT(loginFinished(Answer *)));

    loader->login("admin", "admin");
}

void MainWindow::loginFinished(Answer *answer)
{
    QMessageBox::warning(this, tr("Debug message"), answer->getResult());
}

