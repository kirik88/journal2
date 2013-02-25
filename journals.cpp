#include "journals.h"

Journals::Journals(Loader *loader) : QObject(0)
{
    // сохраняем параметры
    this->loader = loader;

    // инициализируем переменные
    journals = new JournalList();
    classes = new QList<Class *>();
    courses = new QList<Course *>();
    teachers = new QList<User *>();
}

Journals::~Journals()
{
    abort();

    clear();

    delete loader;
    if (journals) delete journals;
    if (classes) delete classes;
    if (courses) delete courses;
    if (teachers) delete teachers;
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

        // очищаем уже имеющийся список журналов
        clearJournals();

        // формируем на основе ответа список журналов
        JournalList *tmp = new JournalList(answer->getValue("journals"));

        // добавляем журналы с список
        for (int i = 0; tmp->count() > i; i++) journals->appendRefresh(tmp->at(i));

        delete tmp;

        return true;
    }

    return false;
}

// создать журнал
bool Journals::createJournal(Journal *&journal, QString *message)
{
    journal->setId(0);

    return saveJournal(journal, message);
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

        // обновляем данные журнала
        journal->setId(answer->getValue("id").toInt());
        journal->changed = QDateTime::fromString(answer->getValue("changed"), dateTimeParseString);

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

// удалить журнал по идентификатору (id)
bool Journals::deleteJournal(int id, QString *message)
{
    // получаем журнал
    Journal *journal;
    if (!getJournal(id, journal, message, false)) return false;

    // помечаем журнал удалённым
    // загружаем журнал через загрузчик
    if (loader->deleteJournal(id))
    {
        Answer *answer = loader->lastAnswer;

        // при ошибке возвращаем сообщение
        if (answer->getCode() != OK)
        {
            if (message) *message = answer->getResult();
            return false;
        };

        return true;
    }
    else // отменили
    {
        return false;
    }
}

// загрузить журнал по его идентификатору (id) с необходимостью перезагрузки его данных (reload)
bool Journals::getJournal(int id, Journal *&journal, QString *message, bool reload)
{
    if (message) *message = "";
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
                if (message) *message = answer->getResult();
                return false;
            };

            // формируем на основе ответа список журналов
            JournalList *tmp = new JournalList(answer->getValue("journals"));

            // запрос должен был вернуть один журнал, если не так - возвращаем ошибку
            if (tmp->count() != 1)
            {
                if (message) *message = tr("Сервер вернул неверное количество журналов.");

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

// обновить/загрузить список классов, предметов и учителей
bool Journals::refreshData(QString *message)
{
    *message = "";

    // загружаем через загрузчик
    if (loader->loadData())
    {
        Answer *answer = loader->lastAnswer;

        // при ошибке возвращаем сообщение
        if (answer->getCode() != OK)
        {
            *message = answer->getResult();
            return false;
        };

        // очищаем уже имеющийся список классов
        clearClasses();

        // формируем на основе ответа список классов
        QDomDocument doc;
        if (doc.setContent(answer->getValue("classes")))
        {
            QDomElement docElem = doc.documentElement();

            // переходим к парсингу
            parseClasses(docElem);
        }

        // формируем на основе ответа список предметов
        if (doc.setContent(answer->getValue("courses")))
        {
            QDomElement docElem = doc.documentElement();

            // переходим к парсингу
            parseCourses(docElem);
        }

        // формируем на основе ответа список учителей
        if (doc.setContent(answer->getValue("teachers")))
        {
            QDomElement docElem = doc.documentElement();

            // переходим к парсингу
            parseTeachers(docElem);
        }

        return true;
    }

    return false;
}

// парсим xml с классами
void Journals::parseClasses(QDomNode node)
{
    while (!node.isNull())
    {
        QDomElement e = node.toElement(); // пробуем преобразовать узел в элемент
        if (!e.isNull())
        {
            // для корневого узла спускаемся ниже
            if (e.tagName() == "classes")
            {
                if (e.hasChildNodes()) parseClasses(node.firstChild());
            }
            // для узлов-журналов
            else if (e.tagName() == "class")
            {
                QString data;
                QTextStream out(&data);
                node.save(out, 1);

                classes->append(new Class(data));

                //qDebug() << "Class: " << classes->at(classes->count() - 1)->getName();
            }
        }
        node = node.nextSibling();
    }
}

// парсим xml с предметами
void Journals::parseCourses(QDomNode node)
{
    while (!node.isNull())
    {
        QDomElement e = node.toElement(); // пробуем преобразовать узел в элемент
        if (!e.isNull())
        {
            // для корневого узла спускаемся ниже
            if (e.tagName() == "courses")
            {
                if (e.hasChildNodes()) parseCourses(node.firstChild());
            }
            // для узлов-журналов
            else if (e.tagName() == "course")
            {
                QString data;
                QTextStream out(&data);
                node.save(out, 1);

                courses->append(new Course(data));

                //qDebug() << "Course: " << courses->at(courses->count() - 1)->getName();
            }
        }
        node = node.nextSibling();
    }
}

// парсим xml с учителями
void Journals::parseTeachers(QDomNode node)
{
    while (!node.isNull())
    {
        QDomElement e = node.toElement(); // пробуем преобразовать узел в элемент
        if (!e.isNull())
        {
            // для корневого узла спускаемся ниже
            if (e.tagName() == "teachers")
            {
                if (e.hasChildNodes()) parseTeachers(node.firstChild());
            }
            // для узлов-журналов
            else if (e.tagName() == "user")
            {
                QString data;
                QTextStream out(&data);
                node.save(out, 1);

                teachers->append(new User(data));

                //qDebug() << "Teacher: " << teachers->at(teachers->count() - 1)->getName();
            }
        }
        node = node.nextSibling();
    }
}

// очистка данных
void Journals::clear()
{
    clearJournals();
    clearClasses();
    clearCourses();
    clearTeachers();
}

// очистка списка журналов
void Journals::clearJournals()
{
    if (!journals) return;

    journals->clear();
}

// очистка списка классов
void Journals::clearClasses()
{
    if (!classes) return;

    while (classes->count() > 0)
    {
        delete classes->at(0);
        classes->removeAt(0);
    }
}

// очистка списка предметов
void Journals::clearCourses()
{
    if (!courses) return;

    while (courses->count() > 0)
    {
        delete courses->at(0);
        courses->removeAt(0);
    }
}

// очистка списка учителей
void Journals::clearTeachers()
{
    if (!teachers) return;

    while (teachers->count() > 0)
    {
        delete teachers->at(0);
        teachers->removeAt(0);
    }
}
