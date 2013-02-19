#include "journaltablewidget.h"

JournalTableWidget::JournalTableWidget(QWidget *parent) :
    QTableWidget(parent)
{
    // отметка "не был"
    markNone = tr("н");

    // назначаем свою отрисовку ячейки
    setItemDelegate(new JournalItemDelegate());
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

    // блокируем сигналы, чтоб не срабатывали в тот момент, когда ячейки только заполняются значениями
    blockSignals(true);

    // списки видимых колонок и строк
    QList<QTableWidgetItem *> cols, rows;

    // формируем список видимых колонок
    for (int col = 0; col < journal->columns.count(); col++)
    {
        Column *column = journal->columns.at(col);

        if (column->isVisible)
        {
            QTableWidgetItem *col_item = new QTableWidgetItem(column->getName());
            col_item->setData(Qt::UserRole, column->getId()); // храним идентификатор колонки
            cols.append(col_item);
        }
    }
    // присваиваем колонкам элементы
    setColumnCount(cols.count());
    for (int col = 0; col < cols.count(); col++)
    {
        setHorizontalHeaderItem(col, cols.at(col));
    }

    // формируем список видимых строк
    for (int rw = 0; rw < journal->rows.count(); rw++)
    {
        Row *row = journal->rows.at(rw);

        QTableWidgetItem *row_item = new QTableWidgetItem(row->getName());
        row_item->setData(Qt::UserRole, row->getId()); // храним идентификатор строки
        rows.append(row_item);
    }
    // присваиваем строкам элементы
    setRowCount(rows.count());
    for (int row = 0; row < rows.count(); row++)
    {
        setVerticalHeaderItem(row, rows.at(row));
    }

    // выставляем значения в ячейки
    for (int col = 0; col < cols.count(); col++)
    {
        for (int row = 0; row < rows.count(); row++)
        {
            int colId = cols.at(col)->data(Qt::UserRole).toInt();
            int rowId = rows.at(row)->data(Qt::UserRole).toInt();

            QTableWidgetItem *item = new QTableWidgetItem();

            Value *val = journal->getValue(colId, rowId);
            if (val)
            {
                QString vstr = val->value;

                if (vstr.toLower() == markNone)
                    item->setData(Qt::UserRole, QPixmap(":/icons/16/markn"));
                else if (1 <= vstr.toInt() && vstr.toInt() <= 5)
                    item->setData(Qt::UserRole, QPixmap(tr(":/icons/16/mark%1").arg(vstr.toInt())));
                else
                    item->setText(vstr);
                    //item->setData(Qt::UserRole, QPixmap(":/icons/16/markNone"));

                // помечаем, есть ли комментарий
                item->setData(Qt::UserRole+1, val->description != "");
            }

            item->setTextAlignment(Qt::AlignCenter);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

            setItem(row, col, item);
        }
    }

    // разблокируем сигналы
    blockSignals(false);

    // выравнивание по содержимому
    resizeColumnsToContents();
    resizeRowsToContents();
}
