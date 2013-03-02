#include "user.h"

User::User(int id, QString login, QString password)
{
    this->id = id;

    this->login = login;
    this->password = password;
    this->name = "";
    this->nameFull = "";
    this->description = "";
    this->userType = utUnknown;
}

User::User(const QString &xml)
{
    this->login = login;
    this->password = password;
    this->name = "";
    this->nameFull = "";
    this->description = "";
    this->userType = utUnknown;

    // загружаем пользователя
    if (xml != "") load(xml);
}

User::User(User *other)
{
    this->id = other->id;

    this->login = other->login;
    this->password = other->password;
    this->name = other->name;
    this->nameFull = other->nameFull;
    this->description = other->description;
    this->userType = other->userType;
}

// загрузка пользователя из файла
bool User::load(QFile *file)
{
    QDomDocument doc;
    if (!doc.setContent(file->readAll())) return false;

    QDomElement docElem = doc.documentElement();

    // переходим к парсингу
    parseNode(docElem);

    return true;
}

// загрузка пользователя из строки
bool User::load(const QString &xml)
{
    QDomDocument doc;
    if (!doc.setContent(xml)) return false;

    QDomElement docElem = doc.documentElement();

    // переходим к парсингу
    parseNode(docElem);

    return true;
}

// вернуть идентификатор пользователя
int User::getId()
{
    return id;
}

// установить идентификатор пользователя
void User::setId(int id)
{
    this->id = id;
}

// вернуть имя пользователя
QString User::getName()
{
    return this->name;
}

// парсим xml
void User::parseNode(QDomNode node)
{
    while (!node.isNull())
    {
        QDomElement e = node.toElement(); // пробуем преобразовать узел в элемент
        if (!e.isNull())
        {
            // для корневого узла
            if (e.tagName() == "user")
            {
                this->id = e.attribute("id").toInt();
                if (e.hasChildNodes()) parseNode(node.firstChild());
            }
            // другие используем
            else if (e.tagName() == "name")
            {
                this->name = e.text();
            }
            else if (e.tagName() == "name_full")
            {
                this->nameFull = e.text();
            }
            else if (e.tagName() == "description")
            {
                this->description = e.text();
            }
        }
        node = node.nextSibling();
    }
}
