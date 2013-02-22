#ifndef COURSE_H
#define COURSE_H

#include <QtXml>
#include <QString>
#include <QDateTime>

class Course
{
public:
    Course(const QString &xml);
    Course(Course *other);

    /* основные данные */
    QString name;
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

#endif // COURSE_H
