#include "loader.h"

Loader::Loader(const QString &site, const QString &storage)
{
    this->site = site;
    this->storage = storage;
}

// авторизоваться в системе
void Loader::login(const QString &login, const QString &password)
{
    // находим md5-хэш пароля
    QString hash = QCryptographicHash::hash(QByteArray().append(password), QCryptographicHash::Md5).toHex();

    // формируем строку запроса
    QUrl url = QString("http://%1/login/%2/%3/%4/xml").arg(site).arg(storage).arg(login).arg(hash);

    reply = network.get(QNetworkRequest(url));
    /*connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
            this, SLOT(httpDataReadProgress(qint64, qint64)));*/

    QObject::connect(reply, SIGNAL(finished()), this, SLOT(httpLoginFinished()));
}

// ответ сервера при логине
void Loader::httpLoginFinished()
{

}

// остановить сетевую активность
void Loader::abort()
{
    httpAborted = true;
}

// загрузить журнал по его идентификатору (id); загрузить полностью все данные журнала (full)
Journal *Loader::loadJournal(int id, bool full)
{
    return NULL;
}
