#include "column.h"

Column::Column(int id, const QString &xml)
{
    this->id = id;

    this->name = "";
    this->date = QDateTime::currentDateTime();
    this->columnTypeId = -1;
    this->description = "";

    // загружаем колонку
    if (xml != "") load(xml);
}

Column::Column(Column *other)
{
    this->id = other->id;

    this->name = other->name;
    this->date = other->date;
    this->columnTypeId = other->columnTypeId;
    this->description = other->description;
}

// загрузка колонки из файла
bool Column::load(QFile *file)
{
    QDomDocument doc;
    if (!doc.setContent(file->readAll())) return false;

    QDomElement docElem = doc.documentElement();

    // переходим к парсингу
    parseNode(docElem);

    return true;
}

// загрузка колонки из строки
bool Column::load(const QString &xml)
{
    QDomDocument doc;
    if (!doc.setContent(xml)) return false;

    QDomElement docElem = doc.documentElement();

    // переходим к парсингу
    parseNode(docElem);

    return true;
}

// вернуть идентификатор колонки
int Column::getId()
{
    return id;
}

// вернуть имя колонки; если оно не задано, имя сформируется автоматически
QString Column::getName()
{
    // вернуть предопределённое имя
    if (this->name != "") return this->name;

    // сформировать имя на основе даты
    return date.toString(defaultColumnName);
}

// парсим xml
void Column::parseNode(QDomNode node)
{
    while (!node.isNull())
    {
        QDomElement e = node.toElement(); // пробуем преобразовать узел в элемент
        if (!e.isNull())
        {
            // для корневого узла
            if (e.tagName() == "column")
            {
                this->id = e.attribute("id").toInt();
                if (e.hasChildNodes()) parseNode(node.firstChild());
            }
            // другие используем
            else if (e.tagName() == "name")
            {
                this->name = e.text();
            }
            else if (e.tagName() == "date")
            {
                this->date = QDateTime::fromString(e.text(), dateTimeParseString);
            }
            else if (e.tagName() == "column_type")
            {
               this->columnTypeId = e.text().toInt();
            }
            else if (e.tagName() == "description")
            {
                this->description = e.text();
            }
        }
        node = node.nextSibling();
    }
}
