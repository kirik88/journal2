#ifndef USER_H
#define USER_H

#include <QtXml>
#include <QString>
#include <QDateTime>

#include "userenums.cpp"

class User
{
public:
    User(int id, QString login, QString password = "");
    User(const QString &xml);
    User(User *other);

    /* данные пользователя  */
    QString login, password; // логин и хешированный md5 пароль
    QString name; // имя пользователя
    QString nameFull; // полное имя пользователя
    QString description; // заметки
    UserType userType; // уровень прав

    /* основные функции */
    bool load(QFile *file);
    bool load(const QString &xml);

    /* функции для работы с внутренними данными */
    int getId();
    void setId(int id);
    QString getName();

private:
    /* внутренние данные пользователя */
    int id;

    /* функция парсинга xml-данных */
    void parseNode(QDomNode node);

};

#endif // USER_H
