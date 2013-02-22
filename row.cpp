#include "row.h"

Row::Row(int id, const QString &xml)
{
    this->id = id;

    this->name = "";

    // загружаем строку
    if (xml != "") load(xml);
}

Row::Row(Row *other)
{
    this->id = other->id;

    this->name = other->name;
}

// загрузка строки из файла
bool Row::load(QFile *file)
{
    QDomDocument doc;
    if (!doc.setContent(file->readAll())) return false;

    QDomElement docElem = doc.documentElement();

    // переходим к парсингу
    parseNode(docElem);

    return true;
}

// загрузка строки из строки
bool Row::load(const QString &xml)
{
    QDomDocument doc;
    if (!doc.setContent(xml)) return false;

    QDomElement docElem = doc.documentElement();

    // переходим к парсингу
    parseNode(docElem);

    return true;
}

// вернуть идентификатор строки
int Row::getId()
{
    return id;
}

// вернуть имя строки
QString Row::getName()
{
    // вернуть предопределённое имя
    return this->name;
}

// парсим xml
void Row::parseNode(QDomNode node)
{
    while (!node.isNull())
    {
        QDomElement e = node.toElement(); // пробуем преобразовать узел в элемент
        if (!e.isNull())
        {
            // для корневого узла
            if (e.tagName() == "row")
            {
                this->id = e.attribute("id").toInt();
                if (e.hasChildNodes()) parseNode(node.firstChild());
            }
            // другие используем
            else if (e.tagName() == "name")
            {
                this->name = e.text();
            }
        }
        node = node.nextSibling();
    }
}
