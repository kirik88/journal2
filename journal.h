#ifndef JOURNAL_H
#define JOURNAL_H

#include <QString>

class Journal
{
public:
    Journal();

    /* основные данные журнала */
    QString title;
    QString notes;

    /* функции для работы с данными */
    void copyFrom(Journal *from);
    void clear();

    /* функции для работы с внутренними данными */
    int getId();

private:
    /* внутренние данные журнала */
    int id;
};

#endif // JOURNAL_H
