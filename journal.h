#ifndef JOURNAL_H
#define JOURNAL_H

#include <QtXml>
#include <QString>

#include "column.h"
#include "row.h"
#include "value.h"

class Journal
{
public:
    Journal(const QString &xml = "");
    Journal(Journal *other);
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
    bool isArchived;
    bool isDeleted;
    bool isChanged; // журнал изменялся с момента загрузки
    bool isNew; // журнал новосозданный

    /* списки данных */
    QList<Column *> columns;
    QList<Row *> rows;
    QList<Value *> values;

    /* основные функции */
    bool load(QFile *file);
    bool load(const QString &xml);
    bool save(QFile *file);

    /* функции для работы с данными */
    void copyFrom(Journal *from);
    void clear();
    Column *getColumn(int id);
    Row *getRow(int id);
    Value *getValue(int colId, int rowId);

    /* функции для работы с внутренними данными */
    int getId();
    void setId(int id);
    QString getName();

private:
    /* внутренние данные журнала */
    int id;

    /* функция парсинга xml-данных */
    void parseNode(QDomNode node);

};

#endif // JOURNAL_H
