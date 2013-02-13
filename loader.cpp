#include "loader.h"

Loader::Loader(const QString &site, const QString &storage) : QObject(0)
{
    // сохраняем параметры
    this->site = site;
    this->storage = storage;

    // инициализируем переменные
    network = new QNetworkAccessManager();
    proxy = new QNetworkProxy();
    httpLoop = 0;
    lastAnswer = 0;
    operation = IDDLE;

    // отлавливаем сигналы
    QObject::connect(network, SIGNAL(finished(QNetworkReply *)), this, SLOT(httpFinished(QNetworkReply *)));
    /*connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
            this, SLOT(httpDataReadProgress(qint64, qint64)));*/
}

Loader::~Loader()
{
    abort();

    delete network;
    if (proxy) delete proxy;
    if (httpLoop) delete httpLoop;
    if (lastAnswer) delete lastAnswer;
}

// авторизоваться в системе
// вернёт false, если операцию отменили
bool Loader::login(bool loop, const QString &login, const QString &password)
{
    if (operation != IDDLE)
    {
        return false; //TODO:вызывать исключение
    }
    operation = LOGINING;
    httpAborted = false;

    // находим md5-хэш пароля
    QString hash = QCryptographicHash::hash(QByteArray().append(password), QCryptographicHash::Md5).toHex();

    // формируем строку запроса
    QUrl url = QString("http://%1/login/%2/%3/%4/xml").arg(site).arg(storage).arg(login).arg(hash);

    // запускаем
    network->get(QNetworkRequest(url));

    // если нужно, "замираем" до конца выполнения запроса
    if (loop)
    {
        httpLoop = new QEventLoop();

        int res = httpLoop->exec();

        delete httpLoop;
        httpLoop = 0;

        return (res == 0);
    }

    return true;
}

// ответ сервера при логине
void Loader::httpFinished(QNetworkReply *reply)
{
    // удаляем предыдущий ответ
    if (lastAnswer)
    {
        delete lastAnswer;
        lastAnswer = 0;
    }

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
                code = NOT_FOUND_HOST;
                errorMessage = tr("Сайт недоступен.");
                break;
            case QNetworkReply::ConnectionRefusedError:
                code = CONNECTION_REFUSED;
                errorMessage = tr("Нет доступа к интернету.");
                break;
            default:
                code = OTHER_ERROR;
                errorMessage = reply->errorString();
            }

            lastAnswer = new Answer(code, errorMessage);
        }
        /*else if (!redirectionTarget.isNull())
        {
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
            lastAnswer = new Answer(reply->readAll());
        }

        // освобождение ресурсов
        reply->deleteLater();
        reply = 0;

        // выпускаем нужный сигнал
        switch (operation)
        {
        case LOGINING:
            emit loginFinished(lastAnswer);
            break;
        default:
            break;
        }

        operation = IDDLE;

        if (httpLoop) httpLoop->exit(0);
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
