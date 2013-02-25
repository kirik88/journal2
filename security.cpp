#include "security.h"

Security::Security()
{
}

// проверка на возможность создания нового журнала
bool Security::allowCreateJournal(User *user)
{
    if (!user) return false;

    // создавать новые журналы разрешается только админу, заучу и учителю
    return (user->userType == utAdmin || user->userType == utDirector || user->userType == utTeacher);
}

// проверка на возможность редактирования журнала
bool Security::allowEditJournal(User *user, Journal *journal)
{
    if (!user || !journal) return false;

    // админу можно всё
    if (user->userType == utAdmin) return true;

    // завуч и учитель может редактировать только неархивированный неудалённый журнал
    if (user->userType == utDirector || user->userType == utTeacher) return (!journal->isDeleted && !journal->isArchived);

    // остальным редактирование запрещено
    return false;
}
