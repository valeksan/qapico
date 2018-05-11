#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QHash>
#include <QPair>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QDebug>

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
        TYPE_PARSE_INFO_PAGE,
        TYPE_PARSE_GITHUB_PAGE,
    };
    enum MainTablesKeys {
        KEY_MAIN_TABLE_CURRENCIES,
        KEY_MAIN_TABLE_CURRENCIES_INFO_URLS,
    };
    enum CurrencyAttrKeys {
        KEY_CYR_NAME_ATTR,
        KEY_CYR_CYMBOL_ATTR,
        KEY_CYR_RANK_ATTR,
        KEY_CYR_PRICE_USD_ATTR,
        KEY_CYR_PRICE_BTC_ATTR,
        KEY_CYR_24H_VOLUME_USD_ATTR,
        KEY_CYR_MARKETCAP_USD_ATTR,
        KEY_CYR_AVAIBLE_SUPPLY_ATTR,
        KEY_CYR_TOTAL_SUPPLY_ATTR,
        KEY_CYR_MAX_SUPPLY_ATTR,
        KEY_CYR_PERCENT_CH_1H_ATTR,
        KEY_CYR_PERCENT_CH_24H_ATTR,
        KEY_CYR_PERCENT_CH_7D_ATTR,
        KEY_CYR_DATE_LAST_UPDATED_ATTR,
    };
    enum MainInfoTablesKeys {
        KEY_INF_LIST_WEBSITE_URLS,
        KEY_INF_LIST_EXPLORER_URLS,
        KEY_INF_LIST_MSG_BOARD_URLS,
        KEY_INF_LIST_CHAT_URLS,
        KEY_INF_LIST_ANNOUNCEMENT_URLS,
        KEY_INF_LIST_SOURCECODE_URLS,
        KEY_INF_LIST_TAGS,
        KEY_INF_LIST_MARKETS,
    };
    enum MarketsArgsKeys {
        KEY_MARKET_SYMBOL = 0,
        KEY_MARKET_NAME,
        KEY_MARKET_EX_SYMBOL,
        KEY_MARKET_VOL24_USD,
        KEY_MARKET_VOL24_BTC,
        KEY_MARKET_VOL_PERC,
        KEY_MARKET_PRICE_USD,
        KEY_MARKET_PRICE_BTC,
        KEY_MARKET_URL,
    };
    enum GithubTablesKeys {
        KEY_GHB_TABLE_PROJECTS,
    };

    enum GithubItemAttrKeys {
        KEY_GHB_CREATED_AT,
        KEY_GHB_UPDATED_AT,
        KEY_GHB_PUSHED_AT,
        KEY_GHB_CLONE_URL,
        KEY_GHB_HOMEPAGE,
        KEY_GHB_LANGUAGE,
        KEY_GHB_LICENSE_NAME,
        KEY_GHB_LICENSE_URL,
        KEY_GHB_IS_FORK,
        KEY_GHB_SIZE,
        KEY_GHB_STARS_CNT,
        KEY_GHB_WATCHERS_CNT,
        KEY_GHB_FORKS_CNT,
        KEY_GHB_OPEN_ISSUES_CNT,
        KEY_GHB_DESCRIPTION,
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
