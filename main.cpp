#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // информация о программе
    a.setOrganizationName("kirik88");
    a.setApplicationName("journal");
    a.setApplicationVersion(appVersion);

    MainWindow w;
    w.show();
    
    return a.exec();
}
