#include "journaltablewidget.h"

JournalTableWidget::JournalTableWidget(QWidget *parent) :
    QTableWidget(parent)
{
    // установка свойств виджета
    this->setMouseTracking(true);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setDragDropOverwriteMode(false);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setGridStyle(Qt::DotLine);
    this->horizontalHeader()->setMinimumSectionSize(25);
    this->verticalHeader()->setMinimumSectionSize(25);

    // отметка "не был"
    markNone = tr("н");

    // назначаем свою отрисовку ячейки
    this->setItemDelegate(new JournalItemDelegate());

    // связываем сигналы
    QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
                this, SLOT(showContextMenu(const QPoint &)));

    // контекстное меню
    context = new QMenu();

    action5 = new QAction(tr("Отметка «5»"), this);
    QObject::connect(action5, SIGNAL(triggered()), SLOT(contextActionTriggered()));
    context->addAction(action5);
    action5->setData("5");

    action4 = new QAction(tr("Отметка «4»"), this);
    QObject::connect(action4, SIGNAL(triggered()), SLOT(contextActionTriggered()));
    context->addAction(action4);
    action4->setData("4");

    action3 = new QAction(tr("Отметка «3»"), this);
    QObject::connect(action3, SIGNAL(triggered()), SLOT(contextActionTriggered()));
    context->addAction(action3);
    action3->setData("3");

    action2 = new QAction(tr("Отметка «2»"), this);
    QObject::connect(action2, SIGNAL(triggered()), SLOT(contextActionTriggered()));
    context->addAction(action2);
    action2->setData("2");

    action1 = new QAction(tr("Отметка «1»"), this);
    QObject::connect(action1, SIGNAL(triggered()), SLOT(contextActionTriggered()));
    context->addAction(action1);
    action1->setData("1");

    context->addSeparator();

    actionComments = new QAction(tr("Комментарии..."), this);
    QObject::connect(actionComments, SIGNAL(triggered()), SLOT(contextActionTriggered()));
    context->addAction(actionComments);

    context->addSeparator();

    actionClear = new QAction(tr("Очистить"), this);
    QObject::connect(actionClear, SIGNAL(triggered()), SLOT(contextActionTriggered()));
    context->addAction(actionClear);
}

// назначает журнал
void JournalTableWidget::setJournal(Journal *journal, bool isReadOnly)
{
    this->journal = journal;
    this->isReadOnly = isReadOnly;

    clearAll();
    fillAll();
}

// отвязывает журнал
void JournalTableWidget::resetJournal()
{
    this->journal = 0;
    this->isReadOnly = true;

    clearAll();
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
            QTableWidgetItem *col_item = new QTableWidgetItem(column->getName() + "\t /" + QString::number(column->getId()));
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

        QTableWidgetItem *row_item = new QTableWidgetItem(row->getName() + "\t /" + QString::number(row->getId()));
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
                // устанавливаем значение
                setItemData(item, val);
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

// установка свойств элементу
void JournalTableWidget::setItemData(QTableWidgetItem *item, Value *value)
{
    if (value)
    {
        QString data = value->value;

        // сохраняем идентификатор значения
        item->setData(Qt::UserRole, value->getId());

        // пробуем преобразовать значение в число
        bool markOk;
        int mark = data.toInt(&markOk);

        if (data.toLower() == markNone)
            item->setData(Qt::UserRole+1, QPixmap(":/icons/16/markn"));
        else if (markOk && 1 <= mark && mark <= 5)
            item->setData(Qt::UserRole+1, QPixmap(tr(":/icons/16/mark%1").arg(mark)));
        else
            item->setText(data);
        //item->setData(Qt::UserRole+1, QPixmap(":/icons/16/markNone"));

        // помечаем, есть ли комментарий
        item->setData(Qt::UserRole+2, value->description != "");
    }
    else
    {
        item->setData(Qt::UserRole, 0);
        item->setData(Qt::UserRole+1, 0);
        item->setData(Qt::UserRole+2, 0);
    }
}

// реакция на вызов контекстного меню
void JournalTableWidget::showContextMenu(const QPoint &)
{
    // защита от дурака
    if (!journal) return;

    // находим ячейку
    QTableWidgetItem *item = this->currentItem();
    if (!item) return;

    // находим колонку ячейки
    QTableWidgetItem *col_item = this->horizontalHeaderItem(item->column());
    if (!col_item) return;
    curCol = col_item->data(Qt::UserRole).toInt();

    // находим запись ячейки
    QTableWidgetItem *row_item = this->verticalHeaderItem(item->row());
    if (!row_item) return;
    curRow = row_item->data(Qt::UserRole).toInt();

    // скрываем элементы редактирования, если журнал открыт в режиме чтения
    action5->setVisible(!isReadOnly);
    action4->setVisible(action5->isVisible());
    action3->setVisible(action5->isVisible());
    action2->setVisible(action5->isVisible());
    action1->setVisible(action5->isVisible());
    actionClear->setVisible(action5->isVisible());

    // скрываем пункт "очистить", если в данной ячейке нет никакого значения
    if (actionClear->isVisible())
    {
        Value *value = journal->getValue(curCol, curRow);
        actionClear->setVisible(value);
    }

    // вызываем контекстное меню
    context->exec(QCursor::pos());
}

// реакция на выбор элемента контекстного меню
void JournalTableWidget::contextActionTriggered()
{
    // узнаём выбранный элемент контекстного меню
    QAction *action = qobject_cast<QAction *>(sender());

    // если ставим отметку
    if (action == action1 || action == action2 || action == action3 || action == action4 || action == action5)
    {
        QString mark = action->data().toString();

        setItemData(this->currentItem(), journal->setValue(curCol, curRow, mark));

        emit journalChanged();
    }
    // если коммертируем
    else if (action == actionComments)
    {
        Value *value = journal->getValue(curCol, curRow);

        // диалоговое окно комментариев
        CommentsDialog *dialog = new CommentsDialog(isReadOnly);
        dialog->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

        // передаём текущее значение
        if (value) dialog->editValue(value);

        // затемняем окно
        //if (useDialogGlass) glass->install(this);

        // отображаем окно
        if (dialog->exec() == QDialog::Accepted)
        {
            setItemData(this->currentItem(), journal->setValueDescription(curCol, curRow, dialog->comments));

            emit journalChanged();
        }

        // убираем затемнение
        //if (!useDialogGlass) glass->remove();

        delete dialog;
    }
    // если очищаем
    else if (action == actionClear)
    {
        journal->clearValue(curCol, curRow);

        setItemData(this->currentItem(), 0);

        emit journalChanged();
    }
}
