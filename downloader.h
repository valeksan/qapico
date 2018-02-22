#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QDir>
#include <QUrl>
#include <QDebug>

#include "exts_funcs.h"

#define DownloadAttributeType static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User+1)
#define DownloadAttributeFilename static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User+2)
#define DownloadAttributeArgs static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User+3)

enum DownloadType
{
    D_TYPE_TEXT = 0,
    D_TYPE_BINARY,
};

enum DownloadErrors
{
    D_NO_ERR = 0,
    D_DISK_ACCESS_ERR = 1000,
};

class Downloader : public QObject
{
    Q_OBJECT

    bool is_unix;

public:
    explicit Downloader(QObject *parent = nullptr);

signals:
    void onComplete(QVariant data, int downloadType, int error, QVariantList args);

public slots:
    void getData(QUrl url, int downloadType = D_TYPE_TEXT, QVariantList args = QVariantList(), QString filename = ""); // Метод инициализации запроса на получение данных
    void onResult(QNetworkReply *reply);    // Слот обработки ответа о полученных данных

private:
    QNetworkAccessManager *manager;         // менеджер сетевого доступа

};


#endif // DOWNLOADER_H
