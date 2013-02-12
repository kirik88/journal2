#ifndef LOADER_H
#define LOADER_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QCryptographicHash>
#include <QtNetwork>

#include "journal.h"
#include "user.h"
#include "answer.h"

#include "loaderenums.cpp"

class Loader : public QObject
{
   Q_OBJECT
public:
    explicit Loader(const QString &site, const QString &storage);
    //explicit Loader(QObject *parent = 0);

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
    LoaderOperation operation; // текущая операция загрузчика

    /* переменные для работы с сетью */
    QNetworkAccessManager *network;
    QEventLoop *httpLoop;

    /* флаги */
    bool httpAborted; // признак остановки сетевой активности

signals:
    /* сигналы по завершении */
    void loginFinished(Answer *answer);

private slots:
    /* ответы сервера */
    void httpFinished(QNetworkReply *reply);

};

#endif // LOADER_H
