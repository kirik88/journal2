#include "journal.h"

Journal::Journal(const QString &xml)
{
    clear();

    // загружаем журнал
    if (xml != "") load(xml);
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
    this->description = from->description;
    this->changed = from->changed;
}

// очистить данные журнала
void Journal::clear()
{
    name = "";
    description = "";
    changed = QDateTime::currentDateTime();
}

// вернуть идентификатор журнала
int Journal::getId()
{
    return id;
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
            else if (e.tagName() == "columns" || e.tagName() == "records" || e.tagName() == "values")
            {
                if (e.hasChildNodes()) parseNode(node.firstChild());
            }
            // другие используем
            else if (e.tagName() == "name")
            {
                this->name = e.text();
            }
            else if (e.tagName() == "description")
            {
                this->description = e.text();
            }
            else if (e.tagName() == "changed")
            {
                this->changed = QDateTime::fromString(e.text(), dateTimeParseString);
            }
            /*else if (e.tagName() == "class")
            {
                this->classId = e.text().toInt();
            }
            else if (e.tagName() == "year")
            {
                this->year = e.text().toInt();
            }
            else if (e.tagName() == "archived")
            {
               this->archived = (e.text().toInt() == 1);
            }
            else if (e.tagName() == "deleted")
            {
               this->deleted = (e.text().toInt() == 1);
            }
            else if (e.tagName() == "column")
            {
                int type = (e.hasAttribute("type") ? e.attribute("type").toInt() : 0);
                int max  = (e.hasAttribute("max")  ? e.attribute("max" ).toInt() : 5);
                appendColumn(e.text(), ColumnType(type), max);
            }
            else if (e.tagName() == "record")
            {
                appendRecord(e.text());
            }
            else if (e.tagName() == "value")
            {
                int col = e.attribute("col").toInt();
                int rec = e.attribute("rec").toInt();
                QString comments = (e.hasAttribute("comments") ? e.attribute("comments") : "");
                appendValue(col, rec, e.text(), comments);
            }*/
        }
        node = node.nextSibling();
    }
}
