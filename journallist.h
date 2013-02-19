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
    JournalList(const QString &xml = "");
    ~JournalList();

    /* основные функции */
    bool load(QFile *file);
    bool load(const QString &xml);

    /* функции для работы со списком */
    void appendRefresh(Journal *journal);
    void clear();

private:
    /* функция парсинга xml-данных */
    void parseNode(QDomNode node);

};

#endif // JOURNALLIST_H
