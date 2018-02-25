#include "downloader.h"

Downloader::Downloader(QObject *parent) : QObject(parent)
{
#ifdef Q_OS_UNIX
    is_unix = true;
#elif Q_OS_WINDOWS
    is_unix = false;
#endif
    // Инициализируем менеджер ...
    m_pManager = new QNetworkAccessManager();

    // ... и подключаем сигнал о завершении получения данных к обработчику полученного ответа
    connect(m_pManager, &QNetworkAccessManager::finished, this, &Downloader::onResult);

    m_downloadListSize = 0;
}

void Downloader::getData(QUrl url, int downloadType, QVariantList args, QString filename)
{
    QNetworkRequest request;    // Отправляемый запрос
    request.setUrl(url);        // Устанавлвиваем URL в запрос
    request.setAttribute(DownloadAttributeType, QVariant::fromValue(static_cast<int>(downloadType)));
    request.setAttribute(DownloadAttributeFilename, QVariant::fromValue(filename));
    request.setAttribute(DownloadAttributeArgs, QVariant::fromValue(args));
    //request.setAttribute(DownloadAttributeIsListItem, args.at(0).ty QVariant::fromValue(args));
    if(args.size() > 0) {
        if(QString(args.first().typeName()) == "DownloaderArg") {
            if(args.first().value<DownloaderArg>().type() == DownloaderArg::TYPE_DOWNLOAD_INDEX) {
                request.setAttribute(DownloadAttributeIsListItem, true);
            } else {
                request.setAttribute(DownloadAttributeIsListItem, false);
            }
            args.removeFirst();
        } else {
            request.setAttribute(DownloadAttributeIsListItem, false);
        }
    } else {
        request.setAttribute(DownloadAttributeIsListItem, false);
    }
    m_pManager->get(request);      // Выполняем запрос
    qDebug() << "getData ..." << request.url();
}

void Downloader::getDataList(QList<QUrl> urls, int downloadType, QVariantList args, QString filename)
{
    if(m_downloadListSize > 0) return;

    m_downloadListSize = urls.size();
    for(int sourceIndex = 0; sourceIndex < urls.size(); sourceIndex++) {
        getData(urls.at(sourceIndex), downloadType, (QVariantList() << QVariant::fromValue(DownloaderArg(DownloaderArg::TYPE_DOWNLOAD_INDEX, QVariant::fromValue(sourceIndex))) << args), filename);
    }
}

void Downloader::onResult(QNetworkReply *reply)
{
    qDebug() << "onResult ...";
    int downloadType = reply->attribute(DownloadAttributeType).toInt();
    auto args = reply->attribute(DownloadAttributeArgs).value<QVariantList>();
    bool isListItem = reply->attribute(DownloadAttributeIsListItem).toBool();
    qDebug() << isListItem;
    // Если в процесе получения данных произошла ошибка
    if(reply->error()){
        // Сообщаем об этом и показываем информацию об ошибках
        qDebug() << "ERROR: " << reply->errorString();
        if(!isListItem) {
            emit onComplete("", downloadType, static_cast<int>(reply->error()), args);
        } else {
            appendDownloadResult("", downloadType, D_REPLY_ERR, (QVariantList() << args << QVariant::fromValue(QPair<QNetworkReply::NetworkError,QString>(reply->error(),reply->errorString()))));
        }
    } else {
        if(downloadType == D_TYPE_TEXT) {
            // В противном случае считываем текст
            QString text = reply->readAll();
            if(!isListItem) {
                emit onComplete(text, downloadType, D_NO_ERR, args);
            } else {
                appendDownloadResult(text, downloadType, D_NO_ERR, args);
            }

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
                    if(!isListItem) {
                        emit onComplete(file->fileName(), downloadType, D_NO_ERR, args); // Посылаем сигнал о завершении получения файла
                    } else {
                        appendDownloadResult(file->fileName(), downloadType, D_NO_ERR, args);
                    }
                }
            } else {
                qDebug() << "ERROR: " << "Disk access error";
                if(!isListItem) {
                    emit onComplete("", downloadType, D_DISK_ACCESS_ERR, args);
                } else {
                    appendDownloadResult("", downloadType, D_DISK_ACCESS_ERR, args);
                }
            }
        }
    }
}
