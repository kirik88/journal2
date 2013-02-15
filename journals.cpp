#include "journals.h"

Journals::Journals(Loader *loader) : QObject(0)
{
    // сохраняем параметры
    this->loader = loader;
}

Journals::~Journals()
{
    delete loader;

    foreach (Journal *journal, journals)
    {
        delete journal;
    }
    journals.clear();
}

// попробовать подключиться к системе под логином (login) и паролем (password), с выводом сообщения об ошибке (message)
// вернёт true только при удачном подключении
bool Journals::tryLogin(const QString &login, const QString &password, QString *message)
{
    *message = "";

    if (loader->login(login, password))
    {
        Answer *answer = loader->lastAnswer;

        if (answer->getCode() != OK)
        {
            *message = answer->getResult();
            return false;
        };

        return true;
    }

    return false;
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
        Journal *loaded = 0;//loader->loadJournal(id);

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
