#ifndef LOADER_H
#define LOADER_H

#include <QString>

#include "journal.h"
#include "user.h"

class Loader
{
public:
    Loader();

    /* функции для работы с журналами */
    Journal *loadJournal(int id, bool full = true);

private:
    /* пользователь, который осуществляет работу с загрузчиком */
    User *user;
};

#endif // LOADER_H
