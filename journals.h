#ifndef JOURNALS_H
#define JOURNALS_H

#include <QList>

#include "journal.h"
#include "loader.h"

class Journals
{
public:
    Journals(Loader *loader);

    /* функции работы с журналами */
    Journal *getJournal(int id, bool reload = true);

private:
    // загрузчик данных
    Loader *loader;

    // список журналов
    QList<Journal *> journals;

};

#endif // JOURNALS_H
