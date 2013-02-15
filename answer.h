#ifndef ANSWER_H
#define ANSWER_H

#include <QtXml>
#include <QObject>
#include <QString>

#include "answerenums.cpp"

class Answer
{
public:
    Answer(const QString &xml);
    Answer(AnswerCode code, const QString &result);

    /* функции получения данных ответа */
    AnswerCode getCode();
    QString getResult();
    QString getValue(const QString &name);

private:
    /* функция парсинга xml-данных */
    void parseNode(QDomNode node);

    /* переменные с данными ответа */
    AnswerCode code; // код ответа
    QString result; // текстовая расшифровка кода
    QMap<QString, QVariant> answer; // массив значений ответа
};

#endif // ANSWER_H
