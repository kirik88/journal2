#ifndef JOURNAL_H
#define JOURNAL_H

#include <QtXml>
#include <QString>

#include "column.h"

class Journal
{
public:
    Journal(const QString &xml);
    ~Journal();

    /* основные данные журнала */
    QString name;
    int classId;
    QString className;
    int teacherId;
    QString teacherName;
    int courseId;
    QString courseName;
    bool isAuto;
    QDateTime changed;
    QString description;
    bool archived;
    bool deleted;
    bool isChanged; // журнал изменялся с момента загрузки
    bool isNew; // журнал новосозданный

    /* списки данных */
    QList<Column *> columns;

    /* основные функции */
    bool load(QFile *file);
    bool load(const QString &xml);

    /* функции для работы с данными */
    void copyFrom(Journal *from);
    void clear();

    /* функции для работы с внутренними данными */
    int getId();
    QString getName();

private:
    /* внутренние данные журнала */
    int id;

    /* функция парсинга xml-данных */
    void parseNode(QDomNode node);

};

#endif // JOURNAL_H
