#ifndef JOURNALTABLEWIDGET_H
#define JOURNALTABLEWIDGET_H

#include <QTableWidget>
#include <QHeaderView>
#include <QMenu>
#include <QContextMenuEvent>
#include <QLayout>
#include <QMessageBox>
#include <QApplication>
#include <QStyle>

#include "journals.h"
#include "journal.h"
#include "journalitemdelegate.h"

#include "commentsdialog.h"
#include "columndialog.h"

// метки для журнала
// будут установлены при инициализации виджета
static QString markN; // "не был"
static QString markB; // "болел"
static QString markP; // "точка"

class JournalTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit JournalTableWidget(QWidget *parent = 0);
    ~JournalTableWidget();

    /* функции работы с виджетами таблицы */
    void updateWidgets();

    /* функции работы с назначением журнала */
    void setJournals(Journals *journals);
    void setJournal(Journal *journal, bool isReadOnly);
    void resetJournal();

private:
    /* внутренние данные */
    Journals *journals;
    Journal *journal;
    bool isReadOnly; // журнал открылся в режиме "только для чтения"

    /* текущие идентификаторы выбранных колонки и строки */
    int curColId;
    int curRowId;

    /* элементы панели инструментов */
    QFrame *frameToolBar;
    QPushButton *buttonCreateColumn;

    /* контекстное меню */
    QMenu *context;
    QAction *action5;
    QAction *action4;
    QAction *action3;
    QAction *action2;
    QAction *action1;
    QAction *actionN;
    QAction *actionB;
    QAction *actionP;
    QAction *actionComments;
    QAction *actionClear;

    /* контекстное меню колонки */
    QMenu *contextColumn;
    QAction *actionColumnEdit;
    QAction *actionColumnDelete;

    /* функции работы с заполнением таблицы */
    void clearAll();
    void fillAll();

    /* вспомогательные функции */
    void setItemData(QTableWidgetItem *item, Value *value);
    void showComments();
    void setNextMark();

    /* колонки */
    void editColumn(int colId);
    void deleteColumn(int colId);

protected:
    /* отлов событий окна */
    bool eventFilter(QObject *obj, QEvent *event);
    
signals:
    /* сигналы при работе с журналом */
    void journalChanged(); // журнал изменился

    /* сигналы для затемнения */
    void installGlass();
    void removeGlass();

private slots:
    /* действия с таблицей */
    void itemDoubleClicked(QTableWidgetItem *item); // двойной клик
    void columnClicked(int col);

    /* панель инструментов */
    void createColumn();

    /* действия с контекстным меню */
    void showContextMenu(const QPoint &);
    void contextActionTriggered();

    /* действия с контекстным меню колонки */
    void columnContext(const QPoint &pos);
    void contextActionColumnTriggered();
    
};

#endif // JOURNALTABLEWIDGET_H
