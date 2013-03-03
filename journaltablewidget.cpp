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

    // панель инструментов
    frameToolBar = new QFrame(parent);
    frameToolBar->setLayout(new QHBoxLayout);
    frameToolBar->layout()->setContentsMargins(10, 0, 10, 0);

    buttonCreateColumn = new QPushButton(frameToolBar);
    buttonCreateColumn->setText(tr(" Добавить колонку..."));
    buttonCreateColumn->setIcon(QIcon(":/icons/16/columnAdd"));
    buttonCreateColumn->setCursor(Qt::PointingHandCursor);
    frameToolBar->layout()->addWidget(buttonCreateColumn);

    QSpacerItem *spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    frameToolBar->layout()->addItem(spacer);

    // отметки
    markN = tr("Н");
    markB = tr("Б");
    markP = tr("•");

    // назначаем свою отрисовку ячейки
    this->setItemDelegate(new JournalItemDelegate());

    // отмечаем, что у колонки тоже есть контекстное меню
    this->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

    // связываем сигналы
    // таблица
    QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
                     this, SLOT(showContextMenu(const QPoint &)));
    QObject::connect(this, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
                     this, SLOT(itemDoubleClicked(QTableWidgetItem *)));
    // панель инструментов
    QObject::connect(buttonCreateColumn, SIGNAL(clicked()),
                     this, SLOT(createColumn()));
    // колонки
    QObject::connect(this->horizontalHeader(), SIGNAL(sectionClicked(int)),
                     this, SLOT(columnClicked(int)));
    QObject::connect(this->horizontalHeader(), SIGNAL(customContextMenuRequested(const QPoint &)),
                     this, SLOT(columnContext(const QPoint &)));

    // будем отлавливать события окна
    this->installEventFilter(this);

    // контекстное меню
    context = new QMenu();

    action5 = new QAction(tr("Отметка «5»"), this);
    QObject::connect(action5, SIGNAL(triggered()), SLOT(contextActionTriggered()));
    context->addAction(action5);
    action5->setData("5");
    action5->setIcon(QIcon(":/icons/16/mark5"));

    action4 = new QAction(tr("Отметка «4»"), this);
    QObject::connect(action4, SIGNAL(triggered()), SLOT(contextActionTriggered()));
    context->addAction(action4);
    action4->setData("4");
    action4->setIcon(QIcon(":/icons/16/mark4"));

    action3 = new QAction(tr("Отметка «3»"), this);
    QObject::connect(action3, SIGNAL(triggered()), SLOT(contextActionTriggered()));
    context->addAction(action3);
    action3->setData("3");
    action3->setIcon(QIcon(":/icons/16/mark3"));

    action2 = new QAction(tr("Отметка «2»"), this);
    QObject::connect(action2, SIGNAL(triggered()), SLOT(contextActionTriggered()));
    context->addAction(action2);
    action2->setData("2");
    action2->setIcon(QIcon(":/icons/16/mark2"));

    action1 = new QAction(tr("Отметка «1»"), this);
    QObject::connect(action1, SIGNAL(triggered()), SLOT(contextActionTriggered()));
    context->addAction(action1);
    action1->setData("1");
    action1->setIcon(QIcon(":/icons/16/mark1"));

    context->addSeparator();

    actionN = new QAction(tr("Поставить метку «не был»"), this);
    QObject::connect(actionN, SIGNAL(triggered()), SLOT(contextActionTriggered()));
    context->addAction(actionN);
    actionN->setData(markN);
    actionN->setIcon(QIcon(":/icons/16/markN"));

    actionB = new QAction(tr("Поставить метку «болел»"), this);
    QObject::connect(actionB, SIGNAL(triggered()), SLOT(contextActionTriggered()));
    context->addAction(actionB);
    actionB->setData(markB);
    actionB->setIcon(QIcon(":/icons/16/markB"));

    actionP = new QAction(tr("Поставить «точку»"), this);
    QObject::connect(actionP, SIGNAL(triggered()), SLOT(contextActionTriggered()));
    context->addAction(actionP);
    actionP->setData(markP);
    actionP->setIcon(QIcon(":/icons/16/markP"));

    context->addSeparator();

    actionComments = new QAction(tr("Комментарии..."), this);
    QObject::connect(actionComments, SIGNAL(triggered()), SLOT(contextActionTriggered()));
    context->addAction(actionComments);

    context->addSeparator();

    actionClear = new QAction(tr("Очистить"), this);
    QObject::connect(actionClear, SIGNAL(triggered()), SLOT(contextActionTriggered()));
    context->addAction(actionClear);
    actionClear->setShortcut(Qt::Key_Delete);

    // контекстное меню колонки
    contextColumn = new QMenu();

    actionColumnEdit = new QAction(tr("Изменить..."), this);
    QObject::connect(actionColumnEdit, SIGNAL(triggered()), SLOT(contextActionColumnTriggered()));
    contextColumn->addAction(actionColumnEdit);

    actionColumnDelete = new QAction(tr("Удалить"), this);
    QObject::connect(actionColumnDelete, SIGNAL(triggered()), SLOT(contextActionColumnTriggered()));
    contextColumn->addAction(actionColumnDelete);
}

JournalTableWidget::~JournalTableWidget()
{
    // контекстное меню
    delete action5;
    delete action4;
    delete action3;
    delete action2;
    delete action1;
    delete actionN;
    delete actionB;
    delete actionP;
    delete actionComments;
    delete actionClear;

    // контекстное меню колонки
    delete actionColumnEdit;
    delete actionColumnDelete;

    // панель инструментов
    delete buttonCreateColumn;
    delete frameToolBar;
}

// расставляет виджеты
void JournalTableWidget::updateWidgets()
{
    QLayout *layout = this->parentWidget()->layout();

    layout->removeWidget(this);
    layout->addWidget(frameToolBar);
    layout->addWidget(this);
}

// назначает ядро
void JournalTableWidget::setJournals(Journals *journals)
{
    this->journals = journals;
}

// назначает журнал
void JournalTableWidget::setJournal(Journal *journal, bool isReadOnly)
{
    this->journal = journal;
    this->isReadOnly = isReadOnly;

    buttonCreateColumn->setVisible(!isReadOnly);

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

    // формируем список колонок
    for (int col = 0; col < journal->columns.count(); col++)
    {
        Column *column = journal->columns.at(col);

        QTableWidgetItem *col_item = new QTableWidgetItem(column->getName() + "\t /" + QString::number(column->getId()));
        col_item->setData(Qt::UserRole, column->getId()); // храним идентификатор колонки
        cols.append(col_item);
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

        if (data.toUpper() == markN)
            item->setData(Qt::UserRole + 1, QPixmap(":/icons/16/markN"));
        else if (data.toUpper() == markB)
            item->setData(Qt::UserRole + 1, QPixmap(":/icons/16/markB"));
        else if (data.toUpper() == markP)
            item->setData(Qt::UserRole + 1, QPixmap(":/icons/16/markP"));
        else if (markOk && 1 <= mark && mark <= 5)
            item->setData(Qt::UserRole + 1, QPixmap(QString(":/icons/16/mark%1").arg(mark)));
        else
        {
            item->setData(Qt::UserRole + 1, 0);
            item->setText(data);
        }
        //item->setData(Qt::UserRole+1, QPixmap(":/icons/16/markNone"));

        // помечаем, есть ли комментарий
        item->setData(Qt::UserRole + 2, value->description != "");
    }
    else
    {
        item->setData(Qt::UserRole, 0);
        item->setData(Qt::UserRole + 1, 0);
        item->setData(Qt::UserRole + 2, 0);
        item->setText("");
    }
}

// создание новой колонки
void JournalTableWidget::createColumn()
{
    // диалоговое окно колонки
    ColumnDialog *dialog = new ColumnDialog(journals);
    dialog->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

    // затемняем окно
    emit installGlass();

    // отображаем окно
    if (dialog->exec() == QDialog::Accepted)
    {
        // создаём колонку и присваиваем ей выбранные свойства
        Column *column = journal->createColumn();
        column->name = dialog->name;
        column->columnTypeId = dialog->columnTypeId;
        column->date = dialog->date;
        column->description = dialog->description;

        // сортируем колонки
        journal->sortColumns();

        // перестраиваем таблицу
        clearAll();
        fillAll();

        emit journalChanged();
    }

    // убираем затемнение
    emit removeGlass();

    delete dialog;
}

// отображение комментариев к отметке элемента
void JournalTableWidget::showComments()
{
    Value *value = journal->getValue(curColId, curRowId);

    // диалоговое окно комментариев
    CommentsDialog *dialog = new CommentsDialog(isReadOnly);
    dialog->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

    // передаём текущее значение
    if (value) dialog->editValue(value);

    // затемняем окно
    emit installGlass();

    // отображаем окно
    if (dialog->exec() == QDialog::Accepted)
    {
        setItemData(this->currentItem(), journal->setValueDescription(curColId, curRowId, dialog->comments));

        emit journalChanged();
    }

    // убираем затемнение
    emit removeGlass();

    delete dialog;
}

// быстрая смена отметки
void JournalTableWidget::setNextMark()
{
    Value *value = journal->getValue(curColId, curRowId);
    if (!value) value = journal->setValue(curColId, curRowId, "");

    int len = 5;
    int marks[] = {5, 4, 3, 2, 1};
    bool markOk;
    int mark = value->value.toInt(&markOk);

    // если уже стоит отметка - выставляем на бал ниже
    if (markOk && 2 <= mark && mark <= 5)
    {
        for (int i = 0; len > i; i++)
        {
            if (marks[i] == mark)
            {
                value->value = QString::number(marks[i + 1]);
                break;
            }
        }
    }
    else
    {
        value->value = QString::number(marks[0]);
    }

    setItemData(this->currentItem(), value);

    emit journalChanged();
}

// изменение колонки
void JournalTableWidget::editColumn(int colId)
{
    // получаем колонку по идентификатору
    Column *column = journal->getColumn(colId);
    if (!column) return;

    // диалоговое окно колонки
    ColumnDialog *dialog = new ColumnDialog(journals);
    dialog->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

    // посылаем в диалог данные выбранной колонки
    dialog->editColumn(column);

    // затемняем окно
    emit installGlass();

    // отображаем окно
    if (dialog->exec() == QDialog::Accepted)
    {
        // создаём колонку и присваиваем ей выбранные свойства
        column->name = dialog->name;
        column->columnTypeId = dialog->columnTypeId;
        column->date = dialog->date;
        column->description = dialog->description;

        // сортируем колонки
        journal->sortColumns();

        // перестраиваем таблицу
        clearAll();
        fillAll();

        emit journalChanged();
    }

    // убираем затемнение
    emit removeGlass();

    delete dialog;
}

// удаление колонки
void JournalTableWidget::deleteColumn(int colId)
{
    // защита от дурака
    if (!journal || isReadOnly) return;

    // получаем колонку по идентификатору
    Column *column = journal->getColumn(colId);
    if (!column) return;

    // подтверждение
    QMessageBox *question = new QMessageBox(QMessageBox::Question, tr("Удаление колонки"),
                                            tr("Вы действительно хотите удалить колонку «%1»?").arg(column->getName()));

    // кнопка "Отмена"
    QPushButton *btn = new QPushButton(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton), tr("Отмена"));
    question->addButton(btn, QMessageBox::RejectRole);

    // кнопка "Удалить"
    btn = new QPushButton(QApplication::style()->standardIcon(QStyle::SP_DialogDiscardButton), tr("Удалить"));
    question->addButton(btn, QMessageBox::AcceptRole);

    // установка затемнения
    emit installGlass();

    // подтвердили
    if (question->exec() == QMessageBox::Accepted)
    {
        // удаляем колонку из журнала
        journal->removeColumn(colId);

        // перестраиваем таблицу
        clearAll();
        fillAll();

        emit journalChanged();
    }

    // убираем затемнение
    emit removeGlass();
}

// отлов событий окна
// используется, чтобы отловить нажатие "горячих клавиш"
bool JournalTableWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyRelease)
    {
        // текущий элемент
        QTableWidgetItem *item = this->currentItem();
        if (!item) return QTableWidget::eventFilter(obj, event);

        // находим колонку ячейки
        QTableWidgetItem *col_item = this->horizontalHeaderItem(item->column());
        if (!col_item) return QTableWidget::eventFilter(obj, event);
        curColId = col_item->data(Qt::UserRole).toInt();

        // находим строку ячейки
        QTableWidgetItem *row_item = this->verticalHeaderItem(item->row());
        if (!row_item) return QTableWidget::eventFilter(obj, event);
        curRowId = row_item->data(Qt::UserRole).toInt();

        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();

        // если журнал открыт на чтение, разрешаем только просмотреть комментарии по пробелу
        if (isReadOnly)
        {
            switch (key)
            {
            // открываем комментарии
            case Qt::Key_Space:
                showComments();
                break;
            }
        }
        else
        {
            if (keyEvent->text().toUpper() == markN) key = Qt::Key_Y; // для русской буквы "Н" при выставлении метки "не был"
            if (keyEvent->text().toUpper() == markB) key = Qt::Key_Comma; // для русской буквы "Б" при выставлении метки ",jktk"
            if (keyEvent->text().toUpper() == ".") key = Qt::Key_Slash; // для "точки"

            switch (key)
            {
            // выставление конкретного значения
            case Qt::Key_1:
                setItemData(this->currentItem(), journal->setValue(curColId, curRowId, "1"));
                emit journalChanged();
                break;
            case Qt::Key_2:
                setItemData(this->currentItem(), journal->setValue(curColId, curRowId, "2"));
                emit journalChanged();
                break;
            case Qt::Key_3:
                setItemData(this->currentItem(), journal->setValue(curColId, curRowId, "3"));
                emit journalChanged();
                break;
            case Qt::Key_4:
                setItemData(this->currentItem(), journal->setValue(curColId, curRowId, "4"));
                emit journalChanged();
                break;
            case Qt::Key_5:
                setItemData(this->currentItem(), journal->setValue(curColId, curRowId, "5"));
                emit journalChanged();
                break;

            // выставление метки "не был"
            case Qt::Key_Y:
                setItemData(this->currentItem(), journal->setValue(curColId, curRowId, markN));
                emit journalChanged();
                break;
            // выставление метки "болел"
            case Qt::Key_Comma:
                setItemData(this->currentItem(), journal->setValue(curColId, curRowId, markB));
                emit journalChanged();
                break;
            // выставление "точки"
            case Qt::Key_Slash:
                setItemData(this->currentItem(), journal->setValue(curColId, curRowId, markP));
                emit journalChanged();
                break;

            // быстрая смена отметки
            case Qt::Key_Space:
                setNextMark();
                break;

            // очистка значения
            case Qt::Key_Backspace:
            case Qt::Key_Delete:
            case Qt::Key_Period:
                journal->clearValue(curColId, curRowId);
                setItemData(this->currentItem(), 0);
                emit journalChanged();
                break;
            }
        }
    }

    return QTableWidget::eventFilter(obj, event);
}

// реакция на двойной клик по ячейке
void JournalTableWidget::itemDoubleClicked(QTableWidgetItem *item)
{
    // защита от дурака
    if (!journal) return;

    // находим колонку ячейки
    QTableWidgetItem *col_item = this->horizontalHeaderItem(item->column());
    if (!col_item) return;
    curColId = col_item->data(Qt::UserRole).toInt();

    // находим строку ячейки
    QTableWidgetItem *row_item = this->verticalHeaderItem(item->row());
    if (!row_item) return;
    curRowId = row_item->data(Qt::UserRole).toInt();

    showComments();
}

// реакция на щелчок по заголовку колонки
void JournalTableWidget::columnClicked(int col)
{
    // защита от дурака
    if (!journal || isReadOnly) return;

    // находим колонку ячейки
    QTableWidgetItem *col_item = this->horizontalHeaderItem(col);
    if (!col_item) return;
    curColId = col_item->data(Qt::UserRole).toInt();

    contextColumn->exec(QCursor::pos());
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
    curColId = col_item->data(Qt::UserRole).toInt();

    // находим строку ячейки
    QTableWidgetItem *row_item = this->verticalHeaderItem(item->row());
    if (!row_item) return;
    curRowId = row_item->data(Qt::UserRole).toInt();

    // скрываем элементы редактирования, если журнал открыт в режиме чтения
    action5->setVisible(!isReadOnly);
    action4->setVisible(action5->isVisible());
    action3->setVisible(action5->isVisible());
    action2->setVisible(action5->isVisible());
    action1->setVisible(action5->isVisible());
    actionN->setVisible(action5->isVisible());
    actionB->setVisible(action5->isVisible());
    actionP->setVisible(action5->isVisible());
    actionClear->setVisible(action5->isVisible());

    // скрываем пункт "очистить", если в данной ячейке нет никакого значения
    if (actionClear->isVisible())
    {
        Value *value = journal->getValue(curColId, curRowId);
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

    // если комментируем
    if (action == actionComments)
    {
        showComments();
    }
    // если очищаем
    else if (action == actionClear)
    {
        journal->clearValue(curColId, curRowId);

        setItemData(this->currentItem(), 0);

        emit journalChanged();
    }
    // если ставим отметку
    else
    {
        QString mark = action->data().toString();

        setItemData(this->currentItem(), journal->setValue(curColId, curRowId, mark));

        emit journalChanged();
    }
}

// реакция на выбор элемента контекстного меню колонки
void JournalTableWidget::contextActionColumnTriggered()
{
    // узнаём выбранный элемент контекстного меню
    QAction *action = qobject_cast<QAction *>(sender());

    // если изменяем
    if (action == actionColumnEdit)
    {
        editColumn(curColId);
    }
    // если удаляем
    else if (action == actionColumnDelete)
    {
        deleteColumn(curColId);
    }
}

// реакция на контекстное меню заголовка колонки
void JournalTableWidget::columnContext(const QPoint &pos) {
    // вычисляем колонку, у которой вызвали контекстное меню
    int col = this->horizontalHeader()->logicalIndexAt(pos);

    // вызываем контекстное меню
    columnClicked(col);
}
