#ifndef USER_H
#define USER_H

#include <QString>

#include "userenums.cpp"

class User
{
public:
    User(int id, QString login, QString password = "");

    /* данные пользователя  */
    int id; // идентификатор
    QString login, password; // логин и хешированный md5 пароль
    QString name; // имя пользователя
    UserType userType; // уровень прав

};

#endif // USER_H
