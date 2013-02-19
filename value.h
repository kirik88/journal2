#ifndef VALUE_H
#define VALUE_H

#include <QtXml>
#include <QString>
#include <QDateTime>

class Value
{
public:
    Value(const QString &xml);
    Value(Value *other);

    /* основные данные */
    int columnId;
    int studentId;
    QString value;
    QString description;

    /* основные функции */
    bool load(QFile *file);
    bool load(const QString &xml);

    /* функции для работы с внутренними данными */
    int getId();

private:
    /* внутренние данные значения */
    int id;

    /* функция парсинга xml-данных */
    void parseNode(QDomNode node);

};

#endif // VALUE_H
