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

    // загрузчик данных
    Loader *loader;

private:
    // список журналов
    QList<Journal *> journals;

};

#endif // JOURNALS_H
