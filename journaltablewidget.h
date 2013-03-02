#ifndef JOURNALTABLEWIDGET_H
#define JOURNALTABLEWIDGET_H

#include <QTableWidget>
#include <QHeaderView>
#include <QMenu>
#include <QContextMenuEvent>
#include <QLayout>

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
    QMenu *context;
    bool isReadOnly; // журнал открылся в режиме "только для чтения"

    /* текущие выбранные колонка и строка */
    int curCol;
    int curRow;

    /* элементы верхней панели */
    QFrame *frameToolBar;
    QPushButton *buttonCreateColumn;

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
    void showComments();
    void setNextMark();

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

    /* верхняя панель */
    void createColumn();

    /* действия с контекстным меню */
    void showContextMenu(const QPoint &);
    void contextActionTriggered();
    
};

#endif // JOURNALTABLEWIDGET_H
