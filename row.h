#ifndef ROW_H
#define ROW_H

#include <QtXml>
#include <QString>
#include <QDateTime>

class Row
{
public:
    Row(const QString &xml);
    Row(Row *other);

    /* основные данные */
    QString name;

    /* основные функции */
    bool load(QFile *file);
    bool load(const QString &xml);

    /* функции для работы с внутренними данными */
    int getId();
    QString getName();

private:
    /* внутренние данные строки */
    int id;

    /* функция парсинга xml-данных */
    void parseNode(QDomNode node);

};

#endif // ROW_H
