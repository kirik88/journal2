#ifndef COLUMN_H
#define COLUMN_H

#include <QtXml>
#include <QString>
#include <QDateTime>

// строка для разбора даты и времени от сервера
static QString dateTimeParseString = "yyyy-MM-dd HH:mm:ss";
static QString defaultColumnName = "d.MM";

class Column
{
public:
    Column(int id, const QString &xml);
    Column(Column *other);

    /* основные данные */
    QString name;
    QDateTime date;
    bool isVisible;
    QString description;

    /* основные функции */
    bool load(QFile *file);
    bool load(const QString &xml);

    /* функции для работы с внутренними данными */
    int getId();
    QString getName();

private:
    /* внутренние данные колонки */
    int id;

    /* функция парсинга xml-данных */
    void parseNode(QDomNode node);

};

#endif // COLUMN_H
