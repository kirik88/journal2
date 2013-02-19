#include "journal.h"

Journal::Journal(const QString &xml)
{
    id = -1;
    clear();

    // загружаем журнал
    if (xml != "") load(xml);
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
    this->archived = from->archived;
    this->deleted = from->deleted;
    this->isChanged = from->isChanged;
    this->isNew = from->isNew;

    // копирование колонок
    for (int i = 0; from->columns.count() > i; i++)
    {
        columns.append(new Column(from->columns.at(i)));
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
    archived = false;
    deleted = false;
    isChanged = false;
    isNew = false;

    // очистка списка колонок
    while (columns.count() > 0)
    {
        delete columns.at(0);
        columns.removeAt(0);
    }
}

// вернуть идентификатор журнала
int Journal::getId()
{
    return id;
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
            else if (e.tagName() == "columns")
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
               this->archived = (e.text().toInt() == 1);
            }
            else if (e.tagName() == "deleted")
            {
               this->deleted = (e.text().toInt() == 1);
            }
            // "вытаскиваем" колонки
            else if (e.tagName() == "column")
            {
                QString data;
                QTextStream out(&data);
                node.save(out, 1);

                columns.append(new Column(data));
            }
        }
        node = node.nextSibling();
    }
}
