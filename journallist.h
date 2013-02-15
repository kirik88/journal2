#ifndef JOURNALLIST_H
#define JOURNALLIST_H

#include <QtXml>
#include <QObject>
#include <QList>
#include <QWidget>

#include "journal.h"

class JournalList : public QList<Journal *>
{
public:
    JournalList(const QString &xml);
    ~JournalList();

    /* функции для работы со списком */
    void clear();

private:
    /* функция парсинга xml-данных */
    void parseNode(QDomNode node);

};

#endif // JOURNALLIST_H
