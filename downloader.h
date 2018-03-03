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

class DownloadResult
{
public:
    DownloadResult() {
        error = 0;
        downloadType = 0;
        data = QVariant();
        complete = false;
    }

    bool empty() {
        return data.isNull();
    }

public:
    int error;
    int errorReply;
    QString errorReplyText;
    QVariant data;
    int downloadType;
    QVariantList args;
    QString url;
    bool complete;

};
Q_DECLARE_METATYPE(DownloadResult)

class Downloader : public QObject
{
    Q_OBJECT

    bool is_unix;

public:
    explicit Downloader(QObject *parent = nullptr, QNetworkAccessManager *manager = nullptr);

    static QByteArray getHtmlPage(QUrl url, int timeout, int &err) {
       Downloader downloader;
       QByteArray page;
       QEventLoop loop;
       QTimer timer;
       timer.setSingleShot(true);
       connect(&downloader, &Downloader::finish, &loop, &QEventLoop::quit);
       connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
       connect(&downloader, &Downloader::complete, [=,&page,&err](DownloadResult result) {
           err = result.error;
           page = result.data.toByteArray();
       });
       downloader.getData(url, Downloader::D_TYPE_TEXT);
       timer.start(timeout);
       loop.exec();
       return page;
    }

    enum DownloadType
    {
        D_TYPE_TEXT = 0,
        D_TYPE_BINARY,
    };

    enum DownloadErrors
    {
        ERR_OK = 0,
        ERR_DISK_ACCESS = 1000,
        ERR_REPLY,
    };

signals:
    void complete(DownloadResult result);
    void progress(uint hash_url, qint64 bytesReceved, qint64 bytesTotal);
    void finish();

public slots:
    void getData(QUrl url, int downloadType = D_TYPE_TEXT, QVariantList args = QVariantList(), QString filename = "");
    void onResult(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_pManager;
    QNetworkReply *m_pCurrentDownload;

};

#endif // DOWNLOADER_H
