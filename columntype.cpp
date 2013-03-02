#include "columntype.h"

ColumnType::ColumnType(const QString &xml) : Item("")
{
    // загружаем колонку
    if (xml != "") load(xml);
}

// парсим xml
void ColumnType::parseNode(QDomNode node)
{
    Item::parseNode(node);

    while (!node.isNull())
    {
        QDomElement e = node.toElement(); // пробуем преобразовать узел в элемент
        if (!e.isNull())
        {
            // для корневого узла
            if (e.tagName() == "column_type")
            {
                this->id = e.attribute("id").toInt();
                if (e.hasChildNodes()) parseNode(node.firstChild());
            }
        }
        node = node.nextSibling();
    }
}
