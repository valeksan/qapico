#include "downloader.h"

Downloader::Downloader(QObject *parent) : QObject(parent)
{
#ifdef Q_OS_UNIX
    is_unix = true;
#elif Q_OS_WINDOWS
    is_unix = false;
#endif
    // Инициализируем менеджер ...
    if(!m_pManager) {
        m_pManager = new QNetworkAccessManager();
    }

    // ... и подключаем сигнал о завершении получения данных к обработчику полученного ответа
    connect(m_pManager, &QNetworkAccessManager::finished, this, &Downloader::onResult);

}

Downloader::Downloader(QNetworkAccessManager *manager, QObject *parent)
{
    m_pManager = manager;
    this->Downloader(parent);
}

void Downloader::getData(QUrl url, int downloadType, QVariantList args, QString filename)
{
    switch (downloadType) {
    case Downloader::D_TYPE_TEXT:
        {
            QNetworkRequest request;
            request.setUrl(url);
            request.setAttribute(DownloadAttributeType, QVariant::fromValue(static_cast<int>(downloadType)));
            request.setAttribute(DownloadAttributeArgs, QVariant::fromValue(args));
            m_pCurrentDownload = m_pManager->get(request);
            qDebug() << "reqDataText(" << request.url() << ")";
        }
        break;
    case Downloader::D_TYPE_BINARY:
        {
            QNetworkRequest request(url);
            request.setUrl(url);
            request.setAttribute(DownloadAttributeType, QVariant::fromValue(static_cast<int>(downloadType)));
            request.setAttribute(DownloadAttributeFilename, QVariant::fromValue(filename));
            request.setAttribute(DownloadAttributeArgs, QVariant::fromValue(args));
            m_pCurrentDownload = m_pManager->get(request);
            connect(currentDownload, &QNetworkReply::downloadProgress, [&](qint64 bytesReceved, qint64 bytesTotal) {
                uint hash_url = qHash(url);
                emit progress(hash_url, bytesReceved, bytesTotal);
            });
            qDebug() << "reqDataFile(" << request.url() << ")";
        }
        break;
    }
}

void Downloader::onResult(QNetworkReply *reply)
{
    qDebug() << "repData(" << reply->url() << ")";

    DownloadResult result;
    result.downloadType = reply->attribute(DownloadAttributeType).toInt();
    result.args = reply->attribute(DownloadAttributeArgs).value<QVariantList>();
    result.errorReply = reply->error();
    result.errorReplyText = reply->errorString();
    result.url = reply->url();

    if(reply->error() == QNetworkReply::NoError) {
        switch(result.downloadType) {
        case Downloader::D_TYPE_TEXT:
            {
                result.data = reply->readAll();
                result.error = Downloader::ERR_OK;
                emit complete(result);
            }
            break;
        case Downloader::D_TYPE_BINARY:
            {
                QString filename = reply->attribute(DownloadAttributeFilename).toString();
                if(isDownloaderPathExists()) {
                    QFile *file = new QFile(QDir::homePath()+QDir::separator()+(is_unix?".apico":"Apico")+QDir::separator()+"Downloads"+QDir::separator()+(filename.isEmpty()?"noname":filename));
                    if(file->open(QFile::WriteOnly)) {
                        file->write(reply->readAll());
                        file->close();
                        qDebug() << "Download " << result.url << " complete";
                        result.data = file->fileName();
                        result.error = Downloader::ERR_OK;
                        emit complete(result);
                    }
                } else {
                    qDebug() << "ERROR: " << "Disk access error";
                    result.data = "";
                    result.error = Downloader::ERR_DISK_ACCESS;
                    emit complete(result);
                }
            }
            break;
        }

    } else {        
        result.data = "";
        result.error = Downloader::ERR_REPLY;
        qDebug() << "ERROR: " << reply->errorString();
        emit complete(result);
    }
}
