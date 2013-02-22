#include "course.h"

Course::Course(const QString &xml)
{
    this->name = "";
    this->description = "";

    // загружаем колонку
    if (xml != "") load(xml);
}

Course::Course(Course *other)
{
    this->id = other->id;

    this->name = other->name;
    this->description = other->description;
}

// загрузка класса из файла
bool Course::load(QFile *file)
{
    QDomDocument doc;
    if (!doc.setContent(file->readAll())) return false;

    QDomElement docElem = doc.documentElement();

    // переходим к парсингу
    parseNode(docElem);

    return true;
}

// загрузка класса из строки
bool Course::load(const QString &xml)
{
    QDomDocument doc;
    if (!doc.setContent(xml)) return false;

    QDomElement docElem = doc.documentElement();

    // переходим к парсингу
    parseNode(docElem);

    return true;
}

// вернуть идентификатор класса
int Course::getId()
{
    return id;
}

// вернуть имя класса
QString Course::getName()
{
    return this->name;
}

// парсим xml
void Course::parseNode(QDomNode node)
{
    while (!node.isNull())
    {
        QDomElement e = node.toElement(); // пробуем преобразовать узел в элемент
        if (!e.isNull())
        {
            // для корневого узла
            if (e.tagName() == "course")
            {
                this->id = e.attribute("id").toInt();
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
        }
        node = node.nextSibling();
    }
}
