#include "journalstreewidget.h"

JournalsTreeWidget::JournalsTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    // установка свойств виджета
    this->setMouseTracking(true);
    this->setIconSize(QSize(32, 32));
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    // колонки
    this->setColumnCount(6);
    this->setHeaderLabels(QStringList()
                          << QObject::tr("Журнал") << QObject::tr("Класс")
                          << QObject::tr("Предмет") << QObject::tr("Учитель")
                          << QObject::tr("Описание") << QObject::tr("Изменён                                        "));
    this->headerItem()->setTextAlignment(5, Qt::AlignRight | Qt::AlignVCenter);

    // запрещаем изменение колонок
    this->header()->setSectionsMovable(false);

    // связываем сигналы
    QObject::connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
            this, SLOT(itemDoubleClicked(QTreeWidgetItem *, int)));

    // контекстное меню
    context = new QMenu();

    actionCreate = new QAction(tr("Создать журнал..."), this);
    QObject::connect(actionCreate, SIGNAL(triggered()), SLOT(buttonCreate_clicked()));
    context->addAction(actionCreate);
    context->addSeparator();

    actionOpen = new QAction(tr("Открыть"), this);
    QObject::connect(actionOpen, SIGNAL(triggered()), SLOT(contextActionTriggered()));
    context->addAction(actionOpen);

    actionEdit = new QAction(tr("Редактировать..."), this);
    QObject::connect(actionEdit, SIGNAL(triggered()), SLOT(contextActionTriggered()));
    context->addAction(actionEdit);

    actionDelete = new QAction(tr("Удалить"), this);
    QObject::connect(actionDelete, SIGNAL(triggered()), SLOT(contextActionTriggered()));
    context->addAction(actionDelete);
}

JournalsTreeWidget::~JournalsTreeWidget()
{
    delete context;
}

// назначает ядро
void JournalsTreeWidget::setJournals(Journals *journals)
{
    this->journals = journals;

    clearAll();
    fillAll();
}

// очищает таблицу
void JournalsTreeWidget::clearAll()
{
    // колонки и строки
    clear();
}

// заполняет таблицу данными журнала
void JournalsTreeWidget::fillAll()
{
    // защита от дурака
    if (!journals || !journals->journals || !journals->loader->user) return;

    for (int i = 0; journals->journals->count() > i; i++)
    {
        Journal *journal = journals->journals->at(i);

        // в зависимости от типа журнала разная иконка
        QIcon icon(journal->isDeleted ? ":/icons/delete" : (journal->isArchived ? ":/icons/journal_archived" : ":/icons/journal"));

        QTreeWidgetItem *item = new QTreeWidgetItem(
                QStringList()
                << tr("%1").arg(journal->getName())
                << journal->className
                << journal->courseName
                << journal->teacherName
                << journal->description);

        this->addTopLevelItem(item);

        // название журнала жирное и с иконкой
        QFont font = item->font(0);
        font.setBold(true);
        item->setFont(0, font);
        item->setIcon(0, icon);

        // класс, предмет и учитель закрашиваем серым цветом
        item->setTextColor(1, QColor(0, 0, 0, 100));
        item->setTextColor(2, QColor(0, 0, 0, 100));
        item->setTextColor(3, QColor(0, 0, 0, 100));

        // описание курсивом
        font = item->font(4);
        font.setItalic(true);
        item->setFont(4, font);

        // дата и время изменения справа
        // кнопка на открытие журнала справа и с иконкой
        // реализовано через фрэйм, что даёт:
        // отступ внутри ячейки, выравнивание по правому краю
        QFrame *frame = new QFrame();
        frame->setLayout(new QHBoxLayout);
        frame->setFrameShape(QFrame::NoFrame);

        QWidget *spacer = new QWidget();
        spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        frame->layout()->addWidget(spacer);

        QLabel *label = new QLabel(journal->changed.toString(tr("d MMM yyyy г., HH:mm")));
        frame->layout()->addWidget(label);

        QToolButton *btn = new QToolButton();
        frame->layout()->addWidget(btn);

        btn->setText(tr("открыть"));
        btn->setObjectName(tr("%1%2").arg(buttonOpenName).arg(journal->getId()));
        //btn->setMaximumWidth(76);
        //btn->setLayoutDirection(Qt::RightToLeft);
        btn->setIcon(QIcon(":/icons/16/open"));
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        btn->setCursor(Qt::PointingHandCursor);

        QObject::connect(btn, SIGNAL(clicked()), this, SLOT(buttonOpen_clicked()));

        item->setData(0, Qt::UserRole, journal->getId());

        this->setItemWidget(item, 5, frame);
    }

    // добавление строки "Новый журнал"
    if (Security::allowCreateJournal(journals->loader->user))
    {
        // иконка
        QIcon icon(":/icons/new");

        QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << tr("Новый журнал"));

        this->addTopLevelItem(item);

        // название
        QFont font = item->font(0);
        font.setBold(true);
        font.setUnderline(true);
        item->setFont(0, font);
        item->setIcon(0, icon);

        // описание
        font = item->font(4);
        font.setItalic(true);
        item->setFont(4, font);

        // кнопка
        QFrame *frame = new QFrame();
        frame->setLayout(new QHBoxLayout);
        frame->setFrameShape(QFrame::NoFrame);

        QWidget *spacer = new QWidget();
        spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        frame->layout()->addWidget(spacer);

        QToolButton *btn = new QToolButton();
        frame->layout()->addWidget(btn);

        btn->setText(tr("создать"));
        btn->setObjectName(tr("%1%2").arg(buttonOpenName).arg(0));
        //btn->setLayoutDirection(Qt::RightToLeft);
        btn->setIcon(QIcon(":/icons/16/plus"));
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        btn->setCursor(Qt::PointingHandCursor);

        QObject::connect(btn, SIGNAL(clicked()), this, SLOT(buttonCreate_clicked()));

        item->setData(0, Qt::UserRole, 0);

        this->setItemWidget(item, 5, frame);
    }

    // выравнивание по содержимому
    for (int j = 0; j < this->header()->count(); j++)
    {
        this->resizeColumnToContents(j);
    }
}

// реакция на вызов контекстного меню
void JournalsTreeWidget::contextMenuEvent(QContextMenuEvent *event)
{
    // защита от дурака
    if (!journals || !journals->loader->user) return;

    if (this->selectedItems().count() > 0)
    {
        User *user = journals->loader->user;
        bool allowCreateJournal = Security::allowCreateJournal(user);

        // узнаём выделенный элемент дерева
        QTreeWidgetItem *item = this->selectedItems().at(0);
        int id = item->data(0, Qt::UserRole).toInt();

        // выставляем видимость пунктов
        actionCreate->setVisible(allowCreateJournal);

        // для строки "Новый журнал" оставляем только пункт создания журнала
        if (id == 0)
        {
            actionOpen->setVisible(false);
            actionEdit->setVisible(false);
            actionDelete->setVisible(false);

            // если создание запрещено - вообще не показываем контекстное меню
            if (!allowCreateJournal) return;
        }
        else
        {
            // получаем журнал, чтоб проверить возможность его редактирования
            Journal *journal;
            journals->getJournal(id, journal, 0, false);
            if (!journal) return;

            actionOpen->setVisible(true);
            actionEdit->setVisible(Security::allowEditJournal(user, journal));
            actionDelete->setVisible(actionEdit->isVisible());
        }

        // вызываем контекстное меню
        context->exec(event->globalPos());
    }
}

// реакция на двойной клик по элементу
void JournalsTreeWidget::itemDoubleClicked(QTreeWidgetItem *item, int)
{
   int id = item->data(0, Qt::UserRole).toInt();

   // создаём новый журнал, если вместо идентификтора число 0
   if (id == 0)
   {
       emit createJournal();
   }
   // иначе загружаем указанный
   else
   {
       emit openJournal(id);
   }
}

// реакция на нажатие кнопки "Открыть" дерева журналов
void JournalsTreeWidget::buttonOpen_clicked()
{
    // получаем имя сработавшей кнопки
    QToolButton *btn = qobject_cast<QToolButton *>(sender());
    QString name = btn->objectName();

    // цифра после имени - идентификатор журнала
    name.remove(0, buttonOpenName.length());
    int id = name.toInt();

    // загружаем выбранный журнал
    emit openJournal(id);
}

// реакция на нажатие кнопки "Новый" дерева журналов
void JournalsTreeWidget::buttonCreate_clicked()
{
    // создаём новый журнал
    emit createJournal();
}

// реакция на выбор элемента контекстного меню
void JournalsTreeWidget::contextActionTriggered()
{
    if (this->selectedItems().count() == 0) return;

    // узнаём выбранный элемент контекстного меню
    QAction *action = qobject_cast<QAction *>(sender());

    // хотим создать новый журнал
    if (action == actionCreate)
    {
        emit createJournal();
    }
    else
    {
        // узнаём выделенный элемент дерева
        QTreeWidgetItem *item = this->selectedItems().at(0);
        int id = item->data(0, Qt::UserRole).toInt();

        // хотим открыть журнал
        if (action == actionOpen)
        {
            emit openJournal(id);
        }
        // хотим изменить журнал
        else if (action == actionEdit)
        {
            emit editJournal(id);
        }
        // хотим удалить журнал
        else if (action == actionDelete)
        {
            emit deleteJournal(id);
        }
    }
}
