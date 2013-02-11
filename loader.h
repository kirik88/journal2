#ifndef LOADER_H
#define LOADER_H

#include "journal.h"

class Loader
{
public:
    Loader();

    /* функции для работы с журналами */
    Journal *loadJournal(int id, bool full = true);
};

#endif // LOADER_H
