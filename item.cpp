#include "item.h"

Item::Item(const QString &xml)
{
    this->name = "";
    this->description = "";

    // загружаем
    if (xml != "") load(xml);
}

Item::~Item() {}

Item::Item(Item *other)
{
    this->id = other->id;

    this->name = other->name;
    this->description = other->description;
}

// загрузка из файла
bool Item::load(QFile *file)
{
    QDomDocument doc;
    if (!doc.setContent(file->readAll())) return false;

    QDomElement docElem = doc.documentElement();

    // переходим к парсингу
    parseNode(docElem);

    return true;
}

// загрузка из строки
bool Item::load(const QString &xml)
{
    QDomDocument doc;
    if (!doc.setContent(xml)) return false;

    QDomElement docElem = doc.documentElement();

    // переходим к парсингу
    parseNode(docElem);

    return true;
}

// вернуть идентификатор
int Item::getId()
{
    return id;
}

// вернуть имя
QString Item::getName()
{
    return this->name;
}

// парсим xml
void Item::parseNode(QDomNode node)
{
    while (!node.isNull())
    {
        QDomElement e = node.toElement(); // пробуем преобразовать узел в элемент
        if (!e.isNull())
        {
            if (e.tagName() == "name")
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
