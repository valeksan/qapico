#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QHash>

#include <qgumbodocument.h>
#include <qgumbonode.h>

typedef QList<QHash<QString,QVariant>> ParseMainPageResult;

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QObject *parent = nullptr);

signals:
    void finish(QVariant result, int stage, int err);

public slots:
    void parse(QString page, int stage = PARSE_MAIN_PAGE);

public:
    enum Stage {
        PARSE_MAIN_PAGE = 0,
        PARSE_CURRENCE_PAGE,
        PARSE_GITHUB_PAGE,
    };
    enum Error {
        ERR_OK = 0,
        ERR_NO_DATA_FOUND,
    };
};

#endif // PARSER_H
