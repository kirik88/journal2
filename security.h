#ifndef SECURITY_H
#define SECURITY_H

#include "user.h"
#include "journal.h"

#include "userenums.cpp"

class Security
{
public:
    Security();

    /* функции проверки прав на действия с журналами */
    static bool allowCreateJournal(User *user);
    static bool allowEditJournal(User *user, Journal *journal);
};

#endif // SECURITY_H
