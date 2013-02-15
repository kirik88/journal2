#ifndef JOURNAL_H
#define JOURNAL_H

#include <QtXml>
#include <QString>

// строка для разбора даты и времени от сервера
static QString dateTimeParseString = "yyyy-MM-dd HH:mm:ss";

class Journal
{
public:
    Journal(const QString &xml);

    /* основные данные журнала */
    QString name;
    QString description;
    QDateTime changed; // дата и время последнего изменения

    /* основные функции */
    bool load(QFile *file);
    bool load(const QString &xml);

    /* функции для работы с данными */
    void copyFrom(Journal *from);
    void clear();

    /* функции для работы с внутренними данными */
    int getId();

private:
    /* внутренние данные журнала */
    int id;

    /* функция парсинга xml-данных */
    void parseNode(QDomNode node);

};

#endif // JOURNAL_H
