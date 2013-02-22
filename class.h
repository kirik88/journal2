#ifndef CLASS_H
#define CLASS_H

#include <QtXml>
#include <QString>
#include <QDateTime>

class Class
{
public:
    Class(const QString &xml);
    Class(Class *other);

    /* основные данные */
    QString name;
    int grade;
    int startYear;
    bool isArchived;
    QString description;

    /* основные функции */
    bool load(QFile *file);
    bool load(const QString &xml);

    /* функции для работы с внутренними данными */
    int getId();
    QString getName();

private:
    /* внутренние данные класса */
    int id;

    /* функция парсинга xml-данных */
    void parseNode(QDomNode node);

};

#endif // CLASS_H
