#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // информация о программе
    a.setOrganizationName("kirik88");
    a.setApplicationName("journal");
    a.setApplicationVersion(appVersion);

    // загрузка русского перевода стандартных надписей
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), ":/lng/");
    a.installTranslator(&qtTranslator);

    MainWindow w;
    w.show();
    
    return a.exec();
}
