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
#define DownloadAttributeIsListItem static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User+4)

enum DownloadType
{
    D_TYPE_TEXT = 0,
    D_TYPE_BINARY,
};

enum DownloadErrors
{
    D_NO_ERR = 0,
    D_DISK_ACCESS_ERR = 1000,
    D_REPLY_ERR,
};

class DownloaderArg
{
public:
    DownloaderArg() {
        m_type = -1;
        m_content = QVariant();
    }

    DownloaderArg(int type, const QVariant &content) {
        m_type = type;
        m_content = content;
    }

    QVariant content() const {
        return m_content;
    }
    int type() const {
        return m_type;
    }

    enum Type {
        TYPE_DOWNLOAD_INDEX,
    };

private:
    int m_type;
    QVariant m_content;
};
Q_DECLARE_METATYPE(DownloaderArg)

class DownloadResult
{
public:
    DownloadResult() {
        m_error = -1;
        m_data = QVariant();
        m_complete = false;
    }

    DownloadResult(const QVariant &data, int downloadType, int error, QVariantList args ) {
        m_data = data;
        m_downloadType = downloadType;
        m_error = error;
        m_args = args;
        m_complete = false;
    }

    QVariant data() const {
        return m_data;
    }

    bool empty() {
        return m_data.isNull();
    }

    int error() const {
        return m_error;
    }

    bool complete() const {
        return m_complete;
    }

    void setComplete(bool complete) {
        m_complete = complete;
    }

    int downloadType() const {
        return m_downloadType;
    }

    QVariantList args() const {
        return m_args;
    }

private:
    int m_error;
    QVariant m_data;
    int m_downloadType;
    QVariantList m_args;
    bool m_complete;

};
Q_DECLARE_METATYPE(DownloadResult)

class Downloader : public QObject
{
    Q_OBJECT

    bool is_unix;

public:
    explicit Downloader(QObject *parent = nullptr);

signals:
    void onComplete(QVariant data, int downloadType, int error, QVariantList args);
    void onCompleteList(QList<QSharedPointer<DownloadResult> > results);

public slots:
    void getData(QUrl url, int downloadType = D_TYPE_TEXT, QVariantList args = QVariantList(), QString filename = ""); // Метод инициализации запроса на получение данных
    void getDataList(QList<QUrl> urls, int downloadType = D_TYPE_TEXT, QVariantList args = QVariantList(), QString filename = ""); // Метод инициализации запроса на получение списка данных
    void onResult(QNetworkReply *reply);    // Слот обработки ответа о полученных данных

private:
    QNetworkAccessManager *m_pManager;         // менеджер сетевого доступа
    QList<QSharedPointer<DownloadResult> > m_resultList;
    int m_downloadListSize;
    int m_downloadCounter;

    void appendDownloadResult(QVariant data, int downloadType, int error = D_NO_ERR, QVariantList args = QVariantList()) {
        m_resultList.append(QSharedPointer<DownloadResult>(new DownloadResult(data, downloadType, error, args)));
        if(m_resultList.size() == m_downloadListSize) {
            emit onCompleteList(m_resultList);
            m_downloadListSize = 0;
            m_resultList.clear();
        }
    }

};
Q_DECLARE_METATYPE(QList<QSharedPointer<DownloadResult> >)

#endif // DOWNLOADER_H
