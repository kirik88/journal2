#ifndef JOURNALS_H
#define JOURNALS_H

#include <QObject>
#include <QString>
#include <QList>

#include "loader.h"
#include "journallist.h"
#include "journal.h"

class Journals : public QObject
{
   Q_OBJECT
public:
    explicit Journals(Loader *loader);
    ~Journals();

    /* основные данные */
    JournalList *journals; // список журналов

    /* основные функции */
    bool tryLogin(const QString &login, const QString &password, QString *message);

    /* функции работы с журналами */
    bool refreshJournals(QString *message);
    Journal *getJournal(int id, bool reload = true);

    /* функции работы с данными */
    void clear();

private:
    /* внутренние данные */
    Loader *loader; // загрузчик

signals:

public slots:

};

#endif // JOURNALS_H
