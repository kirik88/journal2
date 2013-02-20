#include "journals.h"

Journals::Journals(Loader *loader) : QObject(0)
{
    // сохраняем параметры
    this->loader = loader;

    // инициализируем переменные
    journals = new JournalList();
}

Journals::~Journals()
{
    abort();
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

// прервать выполнение длительной операции
void Journals::abort()
{
    loader->abort();
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

        // формируем на основе ответа список журналов
        JournalList *tmp = new JournalList(answer->getValue("journals"));

        // добавляем журналы с список
        for (int i = 0; tmp->count() > i; i++) journals->appendRefresh(tmp->at(i));

        delete tmp;

        return true;
    }

    return false;
}

// загрузить журнал по его идентификатору (id) с необходимостью перезагрузки его данных (reload)
bool Journals::getJournal(int id, Journal *&journal, QString *message, bool reload)
{
    *message = "";
    Journal *result = 0;

    // защита от дурака
    if (!journals) return false;

    // пробуем отыскать журнал в списке
    for (int i = 0; journals->count() > i; i++)
    {
        Journal *jrn = journals->at(i);
        if (jrn->getId() == id)
        {
            result = jrn;
            break;
        }
    }

    // если необходимо, перезагружаем журнал
    if (reload || !result)
    {
        // загружаем журнал через загрузчик
        if (loader->loadJournal(id))
        {
            Answer *answer = loader->lastAnswer;

            // при ошибке возвращаем сообщение
            if (answer->getCode() != OK)
            {
                *message = answer->getResult();
                return false;
            };

            // формируем на основе ответа список журналов
            JournalList *tmp = new JournalList(answer->getValue("journals"));

            // запрос должен был вернуть один журнал, если не так - возвращаем ошибку
            if (tmp->count() != 1)
            {
                *message = tr("Запрос вернул неверное количество журналов.");

                delete tmp;
                return false;
            }

            // получаем журнал из ответа
            Journal *loaded = tmp->at(0);

            // добавляем журнал в список, если его ранее не было
            if (!result)
            {
                journals->append(loaded);
                result = loaded;
            }
            else // иначе обновляем информацию у имеющегося
            {
                result->copyFrom(loaded);
            }

            delete tmp;
        }
        else // отменили
        {
            return false;
        }
    }

    journal = result;

    return result;
}

// сохранить журнал
bool Journals::saveJournal(Journal *&journal, QString *message)
{
    *message = "";

    // защита от дурака
    if (!journals) return false;

    // сохраняем во временный файл
    QTemporaryFile *journalFile = new QTemporaryFile();
    journalFile->open();

    if (!journal->save(journalFile))
    {
        if (journalFile)
        {
            journalFile->close();
            delete journalFile;
            journalFile = 0;
        }

        *message = tr("Произошла ошибка при сохранении журнала во временный файл.");

        return false;
    }
    journalFile->flush();
    journalFile->close();

    // сохраняем журнал через загрузчик
    if (loader->saveJournal(journalFile))
    {
        Answer *answer = loader->lastAnswer;

        // при ошибке возвращаем сообщение
        if (answer->getCode() != OK)
        {
            *message = answer->getResult();

            delete journalFile;
            return false;
        };

        // обновляем в списке либо добавляем сохранённый журнал
        journals->appendRefresh(journal);
    }
    else // отменили
    {
        delete journalFile;
        return false;
    }

    delete journalFile;
    return true;
}

// очистка списка журналов
void Journals::clear()
{
    if (!journals) return;

    journals->clear();
}
