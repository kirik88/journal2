#ifndef JOURNALSTREEWIDGET_H
#define JOURNALSTREEWIDGET_H

#include <QTreeWidget>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QMenu>
#include <QContextMenuEvent>

#include "journals.h"
#include "security.h"

// имя кнопок "Открыть"
static QString buttonOpenName = "buttonOpen"; // кнопка "открыть" дерева журналов

class JournalsTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit JournalsTreeWidget(QWidget *parent = 0);
    ~JournalsTreeWidget();

    /* функции работы со списком журналов */
    void setJournals(Journals *journals);

private:
    /* внутренние данные */
    Journals *journals;
    QMenu *context;

    /* элементы контекстного меню */
    QAction *actionOpen;
    QAction *actionEdit;
    QAction *actionDelete;
    QAction *actionCreate;

    /* функции работы с заполнением дерева */
    void clearAll();
    void fillAll();

protected:
    /* защищённые функции */
    void contextMenuEvent(QContextMenuEvent *);
    
signals:
    /* сигналы действий с журналом */
    void openJournal(int id);
    void editJournal(int id);
    void deleteJournal(int id);
    void createJournal();
    
public slots:
    /* действия с деревом */
    void treeJournals_itemDoubleClicked(QTreeWidgetItem *item, int); // двойной клик
    void buttonOpen_clicked(); // кнопка "Открыть журнал"
    void buttonCreate_clicked(); // кнопка "Новый журнал"

private slots:
    /* внутренние действия */
    void contextActionTriggered();

};

#endif // JOURNALSTREEWIDGET_H
