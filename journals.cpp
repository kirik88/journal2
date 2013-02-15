#include "journals.h"

Journals::Journals(Loader *loader) : QObject(0)
{
    // сохраняем параметры
    this->loader = loader;

    // инициализируем переменные
    journals = 0;
}

Journals::~Journals()
{
    delete loader;
    if (journals) delete journals;
}

// попробовать подключиться к системе под логином (login) и паролем (password), с выводом сообщения об ошибке (message)
// вернёт true только при удачном подключении
bool Journals::tryLogin(const QString &login, const QString &password, QString *message)
{
    *message = "";

    // подключение через загрузчик
    if (loader->login(login, password))
    {
        Answer *answer = loader->lastAnswer;

        // при ошибке возвращаем сообщение
        if (answer->getCode() != OK)
        {
            *message = answer->getResult();
            return false;
        };

        return true;
    }

    return false;
}

// обновить/загрузить список журналов
bool Journals::refreshJournals(QString *message)
{
    *message = "";

    // загружаем через загрузчик
    if (loader->loadJournals())
    {
        Answer *answer = loader->lastAnswer;

        // при ошибке возвращаем сообщение
        if (answer->getCode() != OK)
        {
            *message = answer->getResult();
            return false;
        };

        if (journals) delete journals;

        journals = new JournalList(answer->getValue("journals"));

        return true;
    }

    return false;
}

// загрузить журнал по его идентификатору (id) с необходимостью перезагрузки его данных (reload)
Journal *Journals::getJournal(int id, bool reload)
{
    if (!journals) return 0;

    Journal *journal = 0;

    // пробуем отыскать журнал в списке
    foreach (Journal *journal, *journals)
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
            journals->append(loaded);
            journal = loaded;
        }
        else // иначе обновляем информацию у имеющегося
        {
            journal->copyFrom(loaded);
        }
    }

    return journal;
}

// очистка списка журналов
void Journals::clear()
{
    if (!journals) return;

    journals->clear();
}
