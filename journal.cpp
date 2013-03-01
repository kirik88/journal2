#include "journal.h"

Journal::Journal(const QString &xml)
{
    id = -1;
    clear();

    // загружаем журнал
    if (xml != "") load(xml);
}

Journal::Journal(Journal *other)
{
    this->id = other->id;

    copyFrom(other);
}

Journal::~Journal()
{
    clear();
}

// загрузка журнала из файла
bool Journal::load(QFile *file)
{
    QDomDocument doc;
    if (!doc.setContent(file->readAll())) return false;

    QDomElement docElem = doc.documentElement();

    // переходим к парсингу
    parseNode(docElem);

    return true;
}

// загрузка журнала из строки
bool Journal::load(const QString &xml)
{
    QDomDocument doc;
    if (!doc.setContent(xml)) return false;

    QDomElement docElem = doc.documentElement();

    // переходим к парсингу
    parseNode(docElem);

    return true;
}

// сохранение журнала в файл
bool Journal::save(QFile *file)
{
    QDomDocument doc;
    QDomElement journal = doc.createElement("journal");
    QDomElement element;
    QDomElement child;

    // основные характеристики журнала
    journal.setAttribute("id", this->id);
    if (this->name != "") journal.setAttribute("name", this->name);
    if (this->classId > 0) journal.setAttribute("class", this->classId);
    if (this->teacherId > 0) journal.setAttribute("teacher", this->teacherId);
    if (this->courseId > 0) journal.setAttribute("course", this->courseId);
    journal.setAttribute("auto", this->isAuto ? "1" : "0");
    if (this->description != "") journal.setAttribute("description", this->description);
    journal.setAttribute("deleted", this->isDeleted ? "1" : "0");

    // колонки
    element = doc.createElement("columns");
    for (int col = 0; col < this->columns.count(); col++)
    {
        Column *column = this->columns.at(col);

        child = doc.createElement("column");
        child.setAttribute("id", column->getId());
        if (column->name != "") child.setAttribute("name", column->name);
        child.setAttribute("date", column->date.toString(dateTimeParseString));
        child.setAttribute("visible", column->isVisible ? "1" : "0");
        if (column->description != "") child.setAttribute("description", column->description);
        element.appendChild(child);
    }
    journal.appendChild(element);

    // отметки ученикам
    element = doc.createElement("student_values");
    for (int vl = 0; vl < this->values.count(); vl++)
    {
        Value *val = this->values.at(vl);

        if (val->value != "" || val->description != "")
        {
            child = doc.createElement("student_value");
            child.setAttribute("columnId", val->columnId);
            child.setAttribute("rowId", val->rowId);
            child.setAttribute("value", val->value);
            if (val->description != "") child.setAttribute("description", val->description);
            element.appendChild(child);
        }
    }
    journal.appendChild(element);

    doc.appendChild(journal);

    // устанавливаем кодировку файла
    QDomNode xmlNode = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.insertBefore(xmlNode, doc.firstChild());

    // сохраняем файл
    QTextStream out(file);
    doc.save(out, 1);

    return true;
}

// скопировать данные из другого журнала (from)
void Journal::copyFrom(Journal *from)
{
    clear();

    this->name = from->name;
    this->classId = from->classId;
    this->className = from->className;
    this->teacherId = from->teacherId;
    this->teacherName = from->teacherName;
    this->courseId = from->courseId;
    this->courseName = from->courseName;
    this->isAuto = from->isAuto;
    this->changed = from->changed;
    this->description = from->description;
    this->isArchived = from->isArchived;
    this->isDeleted = from->isDeleted;
    this->isChanged = from->isChanged;
    this->isNew = from->isNew;

    this->nextColumnId = from->nextColumnId;
    this->nextRowId = from->nextRowId;
    this->nextValueId = from->nextValueId;

    // копирование колонок
    for (int i = 0; from->columns.count() > i; i++)
    {
        columns.append(new Column(from->columns.at(i)));
    }

    // копирование строк
    for (int i = 0; from->rows.count() > i; i++)
    {
        rows.append(new Row(from->rows.at(i)));
    }

    // копирование значений
    for (int i = 0; from->values.count() > i; i++)
    {
        values.append(new Value(from->values.at(i)));
    }
}

// очистить данные журнала
void Journal::clear()
{
    name = "";
    classId = -1;
    className = "";
    teacherId = -1;
    teacherName = "";
    courseId = -1;
    courseName = "";
    isAuto = false;
    changed = QDateTime::currentDateTime();
    description = "";
    isArchived = false;
    isDeleted = false;
    isChanged = false;
    isNew = false;

    nextColumnId = 0;
    nextRowId = 0;
    nextValueId = 0;

    // очистка списка колонок
    while (columns.count() > 0)
    {
        delete columns.at(0);
        columns.removeAt(0);
    }

    // очистка списка строк
    while (rows.count() > 0)
    {
        delete rows.at(0);
        rows.removeAt(0);
    }

    // очистка списка значений
    while (values.count() > 0)
    {
        delete values.at(0);
        values.removeAt(0);
    }
}

// вернуть колонку по идентификатору
Column *Journal::getColumn(int id)
{
    for (int col = 0; columns.count() > col; col++)
    {
        Column *column = columns.at(col);

        if (column->getId() == id) return column;
    }

    return 0;
}

// вернуть строку по идентификатору
Row *Journal::getRow(int id)
{
    for (int rw = 0; rows.count() > rw; rw++)
    {
        Row *row = rows.at(rw);

        if (row->getId() == id) return row;
    }

    return 0;
}

// вернуть значение по идентификаторам колонки и строки
Value *Journal::getValue(int colId, int rowId)
{
    for (int val = 0; values.count() > val; val++)
    {
        Value *value = values.at(val);

        if (value->columnId == colId && value->rowId == rowId) return value;
    }

    return 0;
}

// назначить и вернуть значение по идентификаторам колонки и строки
Value *Journal::setValue(int colId, int rowId, QString value)
{
    Value *val = getValue(colId, rowId);

    if (!val)
    {
        val = new Value(getNextValueId(), "");
        values.append(val);
    }

    val->columnId = colId;
    val->rowId = rowId;
    val->value = value;

    return val;
}

// назначить комментарии и вернуть значение по идентификаторам колонки и строки
Value *Journal::setValueDescription(int colId, int rowId, QString description)
{
    Value *val = getValue(colId, rowId);

    if (!val)
    {
        val = new Value(getNextValueId(), "");
        values.append(val);
    }

    val->columnId = colId;
    val->rowId = rowId;
    val->description = description;

    return val;
}

// удалить значение
void Journal::clearValue(int colId, int rowId)
{
    Value *val = getValue(colId, rowId);

    if (val)
    {
        values.removeAll(val);
        delete val;
    }
}

// вернуть идентификатор журнала
int Journal::getId()
{
    return id;
}

// установить идентификатор журнала
void Journal::setId(int id)
{
    this->id = id;
}

// вернуть имя журнала; если оно не задано, имя сформируется автоматически
QString Journal::getName()
{
    // вернуть предопределённое имя
    if (this->name != "") return this->name;

    // сформировать имя на основе наименований класса и предмета
    if (this->className != "" && this->courseName != "") return QString("Класс %1:\n%2").arg(this->className).arg(this->courseName);

    return "";
}

// получить следующий внутренний идентификатор колонки
int Journal::getNextColumnId()
{
    return nextColumnId++;
}

// получить следующий внутренний идентификатор строки
int Journal::getNextRowId()
{
    return nextRowId++;
}

// получить следующий внутренний идентификатор значения
int Journal::getNextValueId()
{
    return nextValueId++;
}

// парсим xml
void Journal::parseNode(QDomNode node)
{
    while (!node.isNull())
    {
        QDomElement e = node.toElement(); // пробуем преобразовать узел в элемент
        if (!e.isNull())
        {
            // для корневого узла
            if (e.tagName() == "journal")
            {
                this->id = e.attribute("id").toInt();
                if (e.hasChildNodes()) parseNode(node.firstChild());
            }
            // для узлов-групп спускаемся ниже
            else if (e.tagName() == "columns" || e.tagName() == "rows" || e.tagName() == "student_values")
            {
                if (e.hasChildNodes()) parseNode(node.firstChild());
            }
            // другие используем
            else if (e.tagName() == "name")
            {
                this->name = e.text();
            }
            else if (e.tagName() == "class")
            {
                this->classId = e.text().toInt();
            }
            else if (e.tagName() == "class_name")
            {
                this->className = e.text();
            }
            else if (e.tagName() == "teacher")
            {
                this->teacherId = e.text().toInt();
            }
            else if (e.tagName() == "teacher_name")
            {
                this->teacherName = e.text();
            }
            else if (e.tagName() == "course")
            {
                this->courseId = e.text().toInt();
            }
            else if (e.tagName() == "course_name")
            {
                this->courseName = e.text();
            }
            else if (e.tagName() == "auto")
            {
               this->isAuto = (e.text().toInt() == 1);
            }
            else if (e.tagName() == "changed")
            {
                this->changed = QDateTime::fromString(e.text(), dateTimeParseString);
            }
            else if (e.tagName() == "description")
            {
                this->description = e.text();
            }
            else if (e.tagName() == "archived")
            {
               this->isArchived = (e.text().toInt() == 1);
            }
            else if (e.tagName() == "deleted")
            {
               this->isDeleted = (e.text().toInt() == 1);
            }
            // "вытаскиваем" колонки
            else if (e.tagName() == "column")
            {
                QString data;
                QTextStream out(&data);
                node.save(out, 1);

                columns.append(new Column(getNextColumnId(), data));
            }
            // "вытаскиваем" строки
            else if (e.tagName() == "row")
            {
                QString data;
                QTextStream out(&data);
                node.save(out, 1);

                rows.append(new Row(getNextRowId(), data));
            }
            // "вытаскиваем" отметки ученикам
            else if (e.tagName() == "student_value")
            {
                QString data;
                QTextStream out(&data);
                node.save(out, 1);

                values.append(new Value(getNextValueId(), data));
            }
        }
        node = node.nextSibling();
    }
}
