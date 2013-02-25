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


// идентификатор формы по-умолчанию при сохранении файла
static QString boundary = "Jrn2kirik88r";

class Loader : public QObject
{
   Q_OBJECT
public:
    explicit Loader(const QString &site, const QString &storage);
    ~Loader();

    /* основные данные */
    Answer *lastAnswer; // последний ответ; загрузчик сам занимается очисткой этой переменной
    QString site; // путь к сайту
    QString storage; // наименование хранилища данных
    User *user; // пользователь, который осуществляет работу с загрузчиком

    /* основные функции */
    bool login(const QString &login, const QString &password, bool loop = true);
    void abort();

    /* функции для работы с журналами */
    bool loadJournals(bool full = false, bool loop = true);
    bool loadJournal(int id, bool loop = true);
    bool saveJournal(QFile *file, bool loop = true);
    bool deleteJournal(int id, bool loop = true);

    /* функции для работы с данными */
    bool loadData(bool loop = true);

private:
    /* внутренние данные */
    LoaderOperation operation; // текущая операция загрузчика

    /* переменные для работы с сетью */
    QNetworkAccessManager *network;
    QNetworkProxy *proxy;
    QEventLoop *httpLoop;
    QNetworkReply *reply;

    /* флаги */
    bool httpAborted; // признак остановки сетевой активности

signals:
    /* сигналы по завершении */
    void loginFinished(Answer *answer); // после подключения к системе
    void journalsFinished(Answer *answer); // после загрузки журналов
    void journalFinished(Answer *answer); // после загрузки журнала
    void saveJournalFinished(Answer *answer); // после сохранения журнала
    void deleteJournalFinished(Answer *answer); // после удаления журнала
    void dataFinished(Answer *answer); // после загрузки данных

private slots:
    /* ответы сервера */
    void httpFinished();

};

#endif // LOADER_H
