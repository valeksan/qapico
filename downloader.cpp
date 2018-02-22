#include "downloader.h"

Downloader::Downloader(QObject *parent) : QObject(parent)
{
#ifdef Q_OS_UNIX
    is_unix = true;
#elif Q_OS_WINDOWS
    is_unix = false;
#endif
    // Инициализируем менеджер ...
    manager = new QNetworkAccessManager();
    // ... и подключаем сигнал о завершении получения данных к обработчику полученного ответа
    connect(manager, &QNetworkAccessManager::finished, this, &Downloader::onResult);
}

void Downloader::getData(QUrl url, int downloadType, QVariantList args, QString filename)
{
    if(downloadType == D_TYPE_TEXT) {
        QNetworkRequest request;    // Отправляемый запрос
        request.setUrl(url);        // Устанавлвиваем URL в запрос
        request.setAttribute(DownloadAttributeType, QVariant::fromValue(static_cast<int>(D_TYPE_TEXT)));
        request.setAttribute(DownloadAttributeFilename, QVariant::fromValue(filename));
        request.setAttribute(DownloadAttributeArgs, QVariant::fromValue(args));
        manager->get(request);      // Выполняем запрос
        qDebug() << "getData ...";
    }
}

void Downloader::onResult(QNetworkReply *reply)
{
    qDebug() << "onResult ...";
    int downloadType = reply->attribute(DownloadAttributeType).toInt();
    auto args = reply->attribute(DownloadAttributeArgs).value<QVariantList>();
    // Если в процесе получения данных произошла ошибка
    if(reply->error()){
        // Сообщаем об этом и показываем информацию об ошибках
        qDebug() << "ERROR: " << reply->errorString();
        emit onComplete("", downloadType, static_cast<int>(reply->error()), args);
    } else {
        if(downloadType == D_TYPE_TEXT) {
            // В противном случае считываем текст
            QString text = reply->readAll();
            emit onComplete(text, downloadType, D_NO_ERR, args);

        } else if(downloadType == D_TYPE_BINARY) {
            // В противном случае создаём объект для работы с файлом
            QString filename = reply->attribute(DownloadAttributeFilename).toString();
            if(isDownloaderPathExists()) {
                QFile *file = new QFile(QDir::homePath()+QDir::separator()+(is_unix?".Apico":"Apico")+QDir::separator()+"Downloads"+QDir::separator()+(filename.isEmpty()?"noname":filename));
                // Создаём файл или открываем его на перезапись ...
                if(file->open(QFile::WriteOnly)){
                    file->write(reply->readAll());  // ... и записываем всю информацию со страницы в файл
                    file->close();                  // закрываем файл
                    qDebug() << "Downloading is completed";
                    emit onComplete(file->fileName(), downloadType, D_NO_ERR, args); // Посылаем сигнал о завершении получения файла
                }
            } else {
                qDebug() << "ERROR: " << "Disk access error";
                emit onComplete("", downloadType, D_DISK_ACCESS_ERR, args);
            }
        }
    }
}
