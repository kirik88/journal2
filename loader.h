#ifndef LOADER_H
#define LOADER_H

#include <QtNetwork>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QCryptographicHash>

#include "journal.h"
#include "user.h"
#include "answer.h"

#include "loaderenums.cpp"

class Loader : public QObject
{
   Q_OBJECT
public:
    explicit Loader(const QString &site, const QString &storage);
    ~Loader();

    /* основные данные */
    Answer *lastAnswer; // последний ответ; загрузчик сам занимается очисткой этой переменной

    /* основные функции */
    bool login(const QString &login, const QString &password, bool loop = true);
    void abort();

    /* функции для работы с журналами */
    bool loadJournals(bool full = false, bool loop = true);
    bool loadJournal(int id, bool full = true, bool loop = true);

private:
    /* внутренние данные */
    QString site; // путь к сайту
    QString storage; // наименование хранилища данных
    User *user; // пользователь, который осуществляет работу с загрузчиком
    LoaderOperation operation; // текущая операция загрузчика

    /* переменные для работы с сетью */
    QNetworkAccessManager *network;
    QNetworkProxy *proxy;
    QEventLoop *httpLoop;

    /* флаги */
    bool httpAborted; // признак остановки сетевой активности

signals:
    /* сигналы по завершении */
    void loginFinished(Answer *answer); // после подключения к системе
    void journalsFinished(Answer *answer); // после загрузки журналов

private slots:
    /* ответы сервера */
    void httpFinished(QNetworkReply *reply);

};

#endif // LOADER_H
