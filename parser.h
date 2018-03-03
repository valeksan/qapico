#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QHash>
#include <QPair>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <qgumbodocument.h>
#include <qgumbonode.h>

#include "parserresult.h"

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QByteArray document = "", int type = TYPE_PARSE_MAIN_PAGE,  QObject *parent = nullptr);

public slots:
    ParserResult parse();

public:
    enum Types {
        TYPE_PARSE_MAIN_PAGE = 0,
        TYPE_PARSE_SUB_PAGE,
        TYPE_PARSE_GITHUB_PAGE,
    };
    enum Error {
        ERR_INIT = -1,
        ERR_OK = 0,        
        ERR_NO_DATA_FOUND,
        ERR_BAD_SYNTAX,
        ERR_UNKNOW_TYPE,
    };

    void setDocument(const QByteArray &document);

    int type() const;
    void setType(int type);

private:
    QByteArray m_document;
    ParserResult m_result;
    int m_type;

    ParserResult _parseMainPage();
    ParserResult _parseSubPage();
    ParserResult _parseGithubPage();

};

#endif // PARSER_H
