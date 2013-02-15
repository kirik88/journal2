#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QCryptographicHash>
#include <QtNetwork>
#include <QMessageBox>
#include <QStyleFactory>
#include <QMovie>

#include "journals.h"

#include "enums.cpp"

// версия приложения
static QString majorVersion = "0";
static QString minorVersion = "01";
static QString appVersion = majorVersion + "." + minorVersion;

// идентификатор формы по-умолчанию при отправлении файла
static QString boundary = "Jrn" + majorVersion + minorVersion + "r";

// путь к файлу с общими настройками
static QString commonIni;

// имя по-умолчанию кнопок и элементов
static QString buttonOpenName = "buttonOpen"; // кнопка "открыть" дерева журналов
static QString itemToolName = "itemTool"; // элемент списка инструментов

// текст надписей
static QString labelBackText; // надпись-ссылка "Вернуться к журналу"
static QString labelRefreshText; // надпись-ссылка "Обновить"

// метка "не был" для журнала
static QString markNone; // будет установлена при инициализации приложения

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

    /* журналы */
    Journals *journals;
    Journal *currentJournal; // текущий выбранный журнал

    /* дополнительно */
    bool useDialogGlass; // использовать затемнение при выводе диалоговых окон
    //Glass *glass; // затемнение окна
    QLabel *labelBack; // надпись-кнопка "Вернуться к журналу"
    QLabel *labelRefresh; // надпись-кнопка "Обновить"
    MainMode currentMode;

    /* панель инструментов */
    QAction *buttonJournal;
    QAction *imageArrow;
    QAction *buttonSave;
    QAction *buttonTools;
    QAction *buttonRefresh;
    QAction *buttonProcessing;

    /* основные функции приложения */
    void prepareApplication();
    void readSettings();
    void connectSignals();
    void changeMainMode(MainMode mode);
    void updateWidgets();
    void fillTree();

private slots:
    void on_buttonLogin_clicked();
    void on_buttonExit_clicked();
    //void loginFinished(Answer *answer);

};

#endif // MAINWINDOW_H
