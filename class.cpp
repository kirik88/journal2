#include "class.h"

Class::Class(const QString &xml)
{
    this->name = "";
    this->grade = -1;
    this->startYear = -1;
    this->isArchived = false;
    this->description = "";

    // загружаем класс
    if (xml != "") load(xml);
}

Class::Class(Class *other)
{
    this->id = other->id;

    this->name = other->name;
    this->grade = other->grade;
    this->startYear = other->startYear;
    this->isArchived = other->isArchived;
    this->description = other->description;
}

// загрузка класса из файла
bool Class::load(QFile *file)
{
    QDomDocument doc;
    if (!doc.setContent(file->readAll())) return false;

    QDomElement docElem = doc.documentElement();

    // переходим к парсингу
    parseNode(docElem);

    return true;
}

// загрузка класса из строки
bool Class::load(const QString &xml)
{
    QDomDocument doc;
    if (!doc.setContent(xml)) return false;

    QDomElement docElem = doc.documentElement();

    // переходим к парсингу
    parseNode(docElem);

    return true;
}

// вернуть идентификатор класса
int Class::getId()
{
    return id;
}

// вернуть имя класса
QString Class::getName()
{
    return this->name;
}

// парсим xml
void Class::parseNode(QDomNode node)
{
    while (!node.isNull())
    {
        QDomElement e = node.toElement(); // пробуем преобразовать узел в элемент
        if (!e.isNull())
        {
            // для корневого узла
            if (e.tagName() == "class")
            {
                this->id = e.attribute("id").toInt();
                if (e.hasChildNodes()) parseNode(node.firstChild());
            }
            // другие используем
            else if (e.tagName() == "name")
            {
                this->name = e.text();
            }
            else if (e.tagName() == "grade")
            {
                this->grade = e.text().toInt();
            }
            else if (e.tagName() == "start_year")
            {
                this->startYear = e.text().toInt();
            }
            else if (e.tagName() == "archived")
            {
               this->isArchived = (e.text().toInt() == 1);
            }
            else if (e.tagName() == "description")
            {
                this->description = e.text();
            }
        }
        node = node.nextSibling();
    }
}
