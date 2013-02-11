#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User(int id, QString login, QString password = "");

    /* идентификационные данные пользователя */
    QString login, password;

private:
    /* внутренние данные пользователя */
    int id;

};

#endif // USER_H
