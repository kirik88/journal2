#ifndef LOADER_H
#define LOADER_H

#include <QString>
#include <QUrl>
#include <QCryptographicHash>
#include <QtNetwork>

#include "journal.h"
#include "user.h"

class Loader
{
public:
    Loader(const QString &site, const QString &storage);

    /* основные функции */
    void login(const QString &login, const QString &password);
    void abort();

    /* функции для работы с журналами */
    Journal *loadJournal(int id, bool full = true);

private:
    /* внутренние данные */
    QString site; // путь к сайту
    QString storage; // наименование хранилища данных
    User *user; // пользователь, который осуществляет работу с загрузчиком

    /* переменные для работы с сетью */
    QNetworkAccessManager network;
    QNetworkReply *reply;
    QEventLoop *httpLoop;

    /* флаги */
    bool httpAborted; // признак остановки сетевой активности

    /* функции ответов сервера */
    void httpLoginFinished();

};

#endif // LOADER_H
