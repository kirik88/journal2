#include "journals.h"

Journals::Journals(Loader *loader)
{
    // сохраняем параметры
    this->loader = loader;
}

Journals::~Journals()
{
    delete loader;
}

// загрузить журнал по его идентификатору (id) с необходимостью перезагрузки его данных (reload)
Journal *Journals::getJournal(int id, bool reload)
{
    Journal *journal = 0;

    // пробуем отыскать журнал в списке
    foreach (Journal *journal, journals)
    {
        if (journal->getId() == id) journal = journal;
    }

    // если необходимо, перезагружаем журнал
    if (reload)
    {
        // загружаем журнал
        Journal *loaded = loader->loadJournal(id);

        // добавляем журнал в список, если его ранее не было
        if (journal)
        {
            journals.append(loaded);
            journal = loaded;
        }
        else // иначе обновляем информацию у имеющегося
        {
            journal->copyFrom(loaded);
        }
    }

    return journal;
}
