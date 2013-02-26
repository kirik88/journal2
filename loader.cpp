#include "loader.h"

Loader::Loader(const QString &site, const QString &storage) : QObject(0)
{
    // сохраняем параметры
    this->site = site;
    this->storage = storage;

    // инициализируем переменные
    user = 0;
    network = new QNetworkAccessManager();
    proxy = new QNetworkProxy();
    httpLoop = 0;
    lastAnswer = 0;
    operation = loIddle;
    reply = 0;

    // отлавливаем сигналы
    QObject::connect(network, SIGNAL(finished(QNetworkReply *)), this, SLOT(httpFinished()));
    /*connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
            this, SLOT(httpDataReadProgress(qint64, qint64)));*/
}

Loader::~Loader()
{
    abort();

    if (user) delete user;

    delete network;
    if (proxy) delete proxy;
    if (httpLoop) delete httpLoop;
    if (lastAnswer) delete lastAnswer;
    if (reply) delete reply;
}

// авторизоваться в системе
// вернёт false, если операцию отменили
bool Loader::login(const QString &login, const QString &password, bool loop)
{
    if (operation != loIddle)
    {
        return false; //TODO:вызывать исключение
    }
    operation = loLogin;
    httpAborted = false;

    // находим md5-хэш пароля
    QString hash = QCryptographicHash::hash(QByteArray().append(password), QCryptographicHash::Md5).toHex();

    // сохраняем данные пользователя
    if (user) delete user;
    user = new User(0, login, hash);

    // формируем строку запроса
    QUrl url = QString("http://%1/login/%2/%3/%4/xml").arg(site).arg(storage).arg(login).arg(hash);

    // запускаем
    reply = network->get(QNetworkRequest(url));

    // если нужно, "замираем" до конца выполнения запроса
    if (loop)
    {
        httpLoop = new QEventLoop();

        int res = httpLoop->exec();

        delete httpLoop;
        httpLoop = 0;

        operation = loIddle;

        return (res == 0);
    }

    return true;
}

// остановить сетевую активность
void Loader::abort()
{
    httpAborted = true;
    if (reply) reply->abort();
}

// загрузить все доступные пользователю журналы
// вернёт false, если операцию отменили
bool Loader::loadJournals(bool full, bool loop)
{
    if (operation != loIddle)
    {
        return false; //TODO:вызывать исключение
    }
    operation = loJournals;
    httpAborted = false;

    // формируем строку запроса
    QUrl url = QString("http://%1/journals%2/xml").arg(site).arg(full ? "full" : "");

    // запускаем
    reply = network->get(QNetworkRequest(url));

    // если нужно, "замираем" до конца выполнения запроса
    if (loop)
    {
        httpLoop = new QEventLoop();

        int res = httpLoop->exec();

        delete httpLoop;
        httpLoop = 0;

        operation = loIddle;

        return (res == 0);
    }

    return true;
}

// загрузить все доступные пользователю данные (классы, предметы и учителей)
// вернёт false, если операцию отменили
bool Loader::loadData(bool loop)
{
    if (operation != loIddle)
    {
        return false; //TODO:вызывать исключение
    }
    operation = loData;
    httpAborted = false;

    // формируем строку запроса
    QUrl url = QString("http://%1/data/xml").arg(site);

    // запускаем
    reply = network->get(QNetworkRequest(url));

    // если нужно, "замираем" до конца выполнения запроса
    if (loop)
    {
        httpLoop = new QEventLoop();

        int res = httpLoop->exec();

        delete httpLoop;
        httpLoop = 0;

        operation = loIddle;

        return (res == 0);
    }

    return true;
}

// загрузить журнал по его идентификатору (id)
// вернёт false, если операцию отменили
bool Loader::loadJournal(int id, bool loop)
{
    if (operation != loIddle)
    {
        return false; //TODO:вызывать исключение
    }
    operation = loJournal;
    httpAborted = false;

    // формируем строку запроса
    QUrl url = QString("http://%1/journal/%2/xml").arg(site).arg(id);

    // запускаем
    reply = network->get(QNetworkRequest(url));

    // если нужно, "замираем" до конца выполнения запроса
    if (loop)
    {
        httpLoop = new QEventLoop();

        int res = httpLoop->exec();

        delete httpLoop;
        httpLoop = 0;

        operation = loIddle;

        return (res == 0);
    }

    return true;
}

// сохранить журнал из файла (file)
// вернёт false, если операцию отменили
bool Loader::saveJournal(QFile *file, bool fullSave, bool loop)
{
    if (operation != loIddle)
    {
        return false; //TODO:вызывать исключение
    }
    operation = loSaveJournal;
    httpAborted = false;

    if (!file->open(QIODevice::ReadOnly)) return false;

    QString bnd = boundary;
    for (int i = 0; i < 10; i++) bnd += tr("%1").arg(qrand() % 10);

    // формируем строку запроса
    //QUrl url = tr("http://mindspace.ru/files/qrcode/qr_card.jpg");
    QUrl url = tr("http://%1/save%2/xml").arg(site).arg(fullSave ? "" : "/nofull");

    // формируем запрос с массивом
    QNetworkRequest request(url);
    request.setRawHeader("Content-Type", QByteArray().append(tr("multipart/form-data; boundary=%1").arg(bnd)));

    // формируем массив данных
    QByteArray array;
    array.append(QByteArray().append(tr("--%1\r\n").arg(bnd)));
    array.append("Content-disposition: form-data; name=\"submit\"\r\n");
    array.append("\r\n");
    array.append(QByteArray().append(tr("Journal 2 Qt-app")));
    array.append("\r\n");
    array.append(QByteArray().append(tr("--%1\r\n").arg(bnd)));
    array.append("Content-disposition: form-data; name=\"file\"; filename=\"journal.xml\"\r\n");
    array.append("Content-Type: text/xml\r\n");
    array.append("\r\n");
    array.append(file->readAll());
    array.append("\r\n");
    array.append(QByteArray().append(tr("--%1--").arg(bnd)));

    file->close();

    // запускаем
    reply = network->post(request, array);

    // если нужно, "замираем" до конца выполнения запроса
    if (loop)
    {
        httpLoop = new QEventLoop();

        int res = httpLoop->exec();

        delete httpLoop;
        httpLoop = 0;

        operation = loIddle;

        return (res == 0);
    }

    return true;
}

// удалить журнал по его идентификатору (id)
// вернёт false, если операцию отменили
bool Loader::deleteJournal(int id, bool loop)
{
    if (operation != loIddle)
    {
        return false; //TODO:вызывать исключение
    }
    operation = loDeleteJournal;
    httpAborted = false;

    // формируем строку запроса
    QUrl url = QString("http://%1/delete/%2/xml").arg(site).arg(id);

    // запускаем
    reply = network->get(QNetworkRequest(url));

    // если нужно, "замираем" до конца выполнения запроса
    if (loop)
    {
        httpLoop = new QEventLoop();

        int res = httpLoop->exec();

        delete httpLoop;
        httpLoop = 0;

        operation = loIddle;

        return (res == 0);
    }

    return true;
}

// стереть журнал по его идентификатору (id)
// вернёт false, если операцию отменили
bool Loader::eraseJournal(int id, bool loop)
{
    if (operation != loIddle)
    {
        return false; //TODO:вызывать исключение
    }
    operation = loEraseJournal;
    httpAborted = false;

    // формируем строку запроса
    QUrl url = QString("http://%1/erase/%2/xml").arg(site).arg(id);

    // запускаем
    reply = network->get(QNetworkRequest(url));

    // если нужно, "замираем" до конца выполнения запроса
    if (loop)
    {
        httpLoop = new QEventLoop();

        int res = httpLoop->exec();

        delete httpLoop;
        httpLoop = 0;

        operation = loIddle;

        return (res == 0);
    }

    return true;
}

// ответ сервера при логине
void Loader::httpFinished()
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
                errorMessage = tr("Сервер с журналами недоступен.");
                break;
            case QNetworkReply::ConnectionRefusedError:
                code = CONNECTION_REFUSED;
                errorMessage = tr("Отсутствует подключение к интернету.");
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
        case loLogin:
            // при успехе сохраняем данные пользователя
            if (lastAnswer->getCode() == OK)
            {
                user->setId(lastAnswer->getValue("id").toInt());
                user->name = lastAnswer->getValue("name");
                user->userType = UserType(lastAnswer->getValue("user_type").toInt());
            }

            emit loginFinished(lastAnswer);
            break;

        case loJournals:
            emit journalsFinished(lastAnswer);
            break;

        case loJournal:
            emit journalFinished(lastAnswer);
            break;

        case loSaveJournal:
            emit saveJournalFinished(lastAnswer);
            break;

        case loDeleteJournal:
            emit deleteJournalFinished(lastAnswer);
            break;

        case loEraseJournal:
            emit eraseJournalFinished(lastAnswer);
            break;

        case loData:
            emit dataFinished(lastAnswer);
            break;

        default:
            break;
        }

        if (httpLoop) httpLoop->exit(0);
   }
}
