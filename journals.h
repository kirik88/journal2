#ifndef JOURNALS_H
#define JOURNALS_H

#include <QObject>
#include <QString>
#include <QList>

#include "journal.h"
#include "loader.h"

class Journals : public QObject
{
   Q_OBJECT
public:
    explicit Journals(Loader *loader);
    ~Journals();

    /* основные функции */
    bool tryLogin(const QString &login, const QString &password, QString *message);

    /* функции работы с журналами */
    Journal *getJournal(int id, bool reload = true);

    // загрузчик данных
    Loader *loader;

private:
    // список журналов
    QList<Journal *> journals;

};

#endif // JOURNALS_H
