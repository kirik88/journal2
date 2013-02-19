#ifndef JOURNALTABLE_H
#define JOURNALTABLE_H

#include <QTableWidget>

#include "journal.h"

class JournalTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit JournalTableWidget(QWidget *parent = 0);

    /* функции работы с назначением журнала */
    void setJournal(Journal *journal);

private:
    /* внутренние данные */
    Journal *journal;

    /* функции работы с заполнением таблицы */
    void clearAll();
    void fillAll();
    
signals:
    
public slots:
    
};

#endif // JOURNALTABLE_H