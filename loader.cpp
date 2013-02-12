#include "loader.h"

Loader::Loader(const QString &site, const QString &storage) : QObject(0)
{
    // сохраняем параметры
    this->site = site;
    this->storage = storage;

    // инициализируем переменные
    network = new QNetworkAccessManager();
    operation = IDDLE;

    // отлавливаем сигналы
    QObject::connect(network, SIGNAL(finished(QNetworkReply *)), this, SLOT(httpFinished(QNetworkReply *)));
    /*connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
            this, SLOT(httpDataReadProgress(qint64, qint64)));*/
}

// авторизоваться в системе
void Loader::login(const QString &login, const QString &password)
{
    if (operation != IDDLE)
    {
        return; //TODO:вызывать исключение
    }

    // находим md5-хэш пароля
    QString hash = QCryptographicHash::hash(QByteArray().append(password), QCryptographicHash::Md5).toHex();

    // формируем строку запроса
    QUrl url = QString("http://%1/login/%2/%3/%4/xml").arg(site).arg(storage).arg(login).arg(hash);

    // запускаем
    network->get(QNetworkRequest(url));
}

// ответ сервера при логине
void Loader::httpFinished(QNetworkReply *reply)
{
    // если прервали
   if (httpAborted)
   {
       reply->deleteLater();
       reply = 0;

       if (httpLoop) httpLoop->exit(1);
   }
   else
   {
       //QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
       // на случай ошибки
       if (reply->error())
       {
           AnswerCode code;
           QString errorMessage;

           switch (reply->error())
           {
           case QNetworkReply::HostNotFoundError:
               code = HOST_NOT_FOUND1;
               errorMessage = tr("сайт недоступен");
               break;
           case QNetworkReply::ConnectionRefusedError:
               code = CONNECTION_REFUSED;
               errorMessage = tr("нет доступа к интернету");
               break;
           default:
               code = OTHER_ERROR;
               errorMessage = reply->errorString();
           }

           emit loginFinished(new Answer(code, errorMessage));

           if (httpLoop) httpLoop->exit(1);
       }
       /*else if (!redirectionTarget.isNull()) {
           QUrl newUrl = url.resolved(redirectionTarget.toUrl());
           if (QMessageBox::question(this, tr("HTTP"),
                                     tr("Redirect to %1 ?").arg(newUrl.toString()),
                                     QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
               url = newUrl;
               reply->deleteLater();
               file->open(QIODevice::WriteOnly);
               file->resize(0);
               startRequest(url);
               return;
           }
       }*/
       else
       {
           Answer *answer = new Answer(reply->readAll());

           emit loginFinished(answer);
       }

       // освобождение ресурсов
       reply->deleteLater();
       reply = 0;
   }
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
