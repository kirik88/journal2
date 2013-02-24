#ifndef JOURNALSTREEWIDGET_H
#define JOURNALSTREEWIDGET_H

#include <QTreeWidget>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>

#include "journals.h"

// имя кнопок "Открыть"
static QString buttonOpenName = "buttonOpen"; // кнопка "открыть" дерева журналов

class JournalsTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit JournalsTreeWidget(QWidget *parent = 0);

    /* функции работы со списком журналов */
    void setJournals(Journals *journals);

    /* функции работы с деревом */
    void setEnableNewButton(bool enable);

private:
    /* внутренние данные */
    Journals *journals;
    bool enableNewButton; // сделать доступным создание нового журнала

    /* функции работы с заполнением дерева */
    void clearAll();
    void fillAll();
    
signals:
    /* сигналы действий с журналом */
    void openJournal(int id);
    void createJournal();
    
public slots:
    /* действия с деревом */
    void treeJournals_itemDoubleClicked(QTreeWidgetItem *item, int); // двойной клик
    void buttonOpen_clicked(); // кнопка "Открыть журнал"
    void buttonCreate_clicked(); // кнопка "Новый журнал"

};

#endif // JOURNALSTREEWIDGET_H
