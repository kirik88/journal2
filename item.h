#ifndef ITEM_H
#define ITEM_H

#include <QtXml>
#include <QString>

class Item
{
public:
    Item(const QString &xml);
    Item(Item *other);

    virtual ~Item();

    /* основные данные */
    QString name;
    QString description;

    /* основные функции */
    bool load(QFile *file);
    bool load(const QString &xml);

    /* функции для работы с внутренними данными */
    int getId();
    QString getName();

protected:
    /* внутренние данные класса */
    int id;

    /* функция парсинга xml-данных */
    virtual void parseNode(QDomNode node);

};

#endif // ITEM_H
