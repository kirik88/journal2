#ifndef COLUMNTYPE_H
#define COLUMNTYPE_H

#include <QtXml>
#include <QString>

#include "item.h"

class ColumnType : public Item
{
public:
    explicit ColumnType(const QString &xml);

protected:
    /* функция парсинга xml-данных */
    void parseNode(QDomNode node);

};

#endif // COLUMNTYPE_H
