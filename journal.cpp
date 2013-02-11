#include "journal.h"

Journal::Journal()
{
    clear();
}

// скопировать данные из другого журнала (from)
void Journal::copyFrom(Journal *from)
{
    clear();

    this->title = from->title;
    this->notes = from->notes;
}

// очистить данные журнала
void Journal::clear()
{
    title = "";
    notes = "";
}

// вернуть идентификатор журнала
int Journal::getId()
{
    return id;
}
