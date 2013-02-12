#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QCryptographicHash>
#include <QtNetwork>
#include <QMessageBox>

#include "loader.h"

// версия приложения
static QString majorVersion = "0";
static QString minorVersion = "01";
static QString appVersion = majorVersion + "." + minorVersion;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;

private slots:
    void buttonClick();
    void loginFinished(Answer *answer);

};

#endif // MAINWINDOW_H
