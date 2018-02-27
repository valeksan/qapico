#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QHash>

#include <qgumbodocument.h>
#include <qgumbonode.h>

#include "core.h"
#include "downloader.h"
#include "parserresult.h"

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(int maxDownloadThreads = 6, QObject *parent = nullptr);

signals:
    void progress(int percent);
    void finish(QVariant result);

public slots:
    void start();
    void nextStage();

public:
    enum Stage {
        STAGE_DOWNLOAD_MAIN_PAGE = 0,
        STAGE_PARSE_MAIN_PAGE,
        STAGE_DOWNLOAD_CURRENCE_PAGES,
        STAGE_PARSE_CURRENCE_PAGES,
        STAGE_DOWNLOAD_GITHUB_PAGES,
        STAGE_PARSE_GITHUB_PAGES,
    };
    enum Error {
        ERR_OK = 0,
        ERR_NO_DATA_FOUND,
    };

private:
    Core *m_pCore;
    QNetworkAccessManager *m_pNetworkAccessManager;
    int m_stage;
    ParserResult *m_pResult;

    void registerAllTasks();

};

#endif // PARSER_H
