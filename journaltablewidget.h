#ifndef JOURNALTABLEWIDGET_H
#define JOURNALTABLEWIDGET_H

#include <QTableWidget>
#include <QHeaderView>
#include <QMenu>
#include <QContextMenuEvent>

#include "journal.h"
#include "journalitemdelegate.h"
#include "commentsdialog.h"

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

    /* функции работы с назначением журнала */
    void setJournal(Journal *journal, bool isReadOnly);
    void resetJournal();

private:
    /* внутренние данные */
    Journal *journal;
    QMenu *context;
    bool isReadOnly; // журнал открылся в режиме "только для чтения"

    /* текущие выбранные колонка и строка */
    int curCol;
    int curRow;

    /* элементы контекстного меню */
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

    /* функции работы с заполнением таблицы */
    void clearAll();
    void fillAll();

    /* вспомогательные функции */
    void setItemData(QTableWidgetItem *item, Value *value);
    
signals:
    /* сигналы при работе с журналом */
    void journalChanged(); // журнал изменился
    
public slots: 

private slots:
    /* внутренние действия */
    void showContextMenu(const QPoint &);
    void contextActionTriggered();
    
};

#endif // JOURNALTABLEWIDGET_H
