#ifndef JOURNALS_H
#define JOURNALS_H

#include <QObject>
#include <QString>
#include <QList>

#include "loader.h"
#include "journallist.h"
#include "journal.h"
#include "class.h"
#include "course.h"
#include "user.h"

class Journals : public QObject
{
   Q_OBJECT
public:
    explicit Journals(Loader *loader);
    ~Journals();

    /* основные данные */
    Loader *loader; // загрузчик
    JournalList *journals; // список журналов
    QList<Class *> *classes; // список классов
    QList<Course *> *courses; // список предметов
    QList<User *> *teachers; // список учителей

    /* основные функции */
    bool tryLogin(const QString &login, const QString &password, QString *message);
    void abort();

    /* функции работы с журналами */
    bool refreshJournals(QString *message);
    bool createJournal(Journal *&journal, QString *message);  
    bool saveJournal(Journal *&journal, QString *message);
    bool deleteJournal(int id, QString *message);
    bool getJournal(int id, Journal *&journal, QString *message, bool reload = true);

    /* функции работы с данными */
    bool refreshData(QString *message);

    /* функции парсинга данных */
    void parseClasses(QDomNode node);
    void parseCourses(QDomNode node);
    void parseTeachers(QDomNode node);

    /* функции очистки данных */
    void clear();
    void clearJournals();
    void clearClasses();
    void clearCourses();
    void clearTeachers();

private:

signals:

public slots:

};

#endif // JOURNALS_H
