#include "journaltablewidget.h"

JournalTableWidget::JournalTableWidget(QWidget *parent) :
    QTableWidget(parent)
{
}

// назначает журнал
void JournalTableWidget::setJournal(Journal *journal)
{
    this->journal = journal;

    clearAll();
    fillAll();
}

// очищает таблицу
void JournalTableWidget::clearAll()
{
    // колонки и строки
    clear();
    setColumnCount(0);
    setRowCount(0);
}

// заполняет таблицу данными журнала
void JournalTableWidget::fillAll()
{
    // защита от дурака
    if (!journal) return;

    // колонки и строки
    setColumnCount(journal->columns.count());
    //setRowCount(journal->recordsCount());

    // заголовки колонок
    for (int col = 0; col < journal->columns.count(); col++)
    {
        Column *column = journal->columns.at(col);

        QTableWidgetItem *col_item = new QTableWidgetItem(column->getName());
        col_item->setData(Qt::UserRole, column->getId()); // храним идентификатор колонки
        setHorizontalHeaderItem(col, col_item);
    }

    // текст записей
    //for (int rec = 0; rec < journal->recordsCount(); rec++)
    //{
    //    QTableWidgetItem *rec_item = new QTableWidgetItem(journal->record(rec)->text);
    //    rec_item->setData(Qt::UserRole, journal->record(rec)->id); // храним идентификатор записи
    //    tableJournal->setVerticalHeaderItem(rec, rec_item);
    //}

    // значения
    //fillValues();
}
