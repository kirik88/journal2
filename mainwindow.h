#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QCryptographicHash>
#include <QtNetwork>
#include <QMessageBox>
#include <QStyleFactory>
#include <QMovie>
#include <QCloseEvent>

#include "glass.h"
#include "journals.h"
#include "journaltablewidget.h"

#include "enums.cpp"

// версия приложения
static QString majorVersion = "0";
static QString minorVersion = "2";
static QString appVersion = majorVersion + "." + minorVersion;

// путь к файлу с общими настройками
static QString commonIni;

// имя по-умолчанию кнопок и элементов
static QString buttonOpenName = "buttonOpen"; // кнопка "открыть" дерева журналов
static QString itemToolName = "itemTool"; // элемент списка инструментов

// текст надписей
static QString labelBackText; // надпись-ссылка "Вернуться к журналу"
static QString labelRefreshText; // надпись-ссылка "Обновить"

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

    /* основные виджеты */
    JournalTableWidget *tableJournal; // таблица журнала

    /* дополнительные */
    Glass *glass; // затемнение окна
    QLabel *labelBack; // надпись-кнопка "Вернуться к журналу"
    QLabel *labelRefresh; // надпись-кнопка "Обновить"
    QMovie *loading; // "крутящиеся колёсики", анимация загрузки

    /* вспомогательные данные */
    bool useDialogGlass; // использовать затемнение при выводе диалоговых окон
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
    bool eventFilter(QObject *obj, QEvent *event);
    void readSettings();
    void writeSettings(Setting setting);
    void connectSignals();
    void checkButtons();
    void changeMainMode(MainMode mode);
    void updateWidgets();
    void updateWindowTitle();
    void fillTree();
    bool checkSaveJournal(const QString &text, bool allowSave = true);
    bool checkAllowEditJournal(Journal *journal);

private slots:
    /* кнопки режима подключения к системе */
    void on_buttonLogin_clicked();
    void on_buttonLoginCancel_clicked();

    /* кнопки на панели инструментов */
    void on_buttonJournals_clicked();
    void on_buttonJournal_clicked();
    void on_buttonSave_clicked();
    void on_buttonRefresh_clicked();
    void on_buttonAbout_clicked();
    void on_buttonExit_clicked();

    /* кнопки дерева журналов */
    void labelBack_clicked(); // надпись-кнопка "Вернуться к журналу"
    void labelRefresh_clicked(); // надпись-кнопка "Обновить"
    void buttonOpen_clicked(); // кнопка "Открыть"

    /* таблица данных журнала */
    void journal_changed();

protected:
    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
