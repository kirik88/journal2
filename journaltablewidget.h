#ifndef JOURNALTABLEWIDGET_H
#define JOURNALTABLEWIDGET_H

#include <QTableWidget>
#include <QHeaderView>

#include "journal.h"
#include "journalitemdelegate.h"

// метка "не был" для журнала
static QString markNone; // будет установлена при инициализации приложения

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
    /* сигналы при работе с журналом */
    void journalChanged(); // журнал изменился
    
public slots: 
    
};

#endif // JOURNALTABLEWIDGET_H
