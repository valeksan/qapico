#ifndef DATABASE_NAMES_H
#define DATABASE_NAMES_H

/* Директивы имен таблицы, полей таблицы и базы данных */
#define DATABASE_HOSTNAME       "ApicoDataBase"
#define DATABASE_NAME           "DataBase.db"

//////////////////////////////////////////////////////////////////////////
/// Таблица: Проекты
//////////////////////////////////////////////////////////////////////////
#define T_CURRENCIES                        "TableCurrencies"
// fast updateble cells
#define CELL_CURRENCIES_NAME                    "Name"
#define CELL_CURRENCIES_SYMBOL                  "Symbol"
#define CELL_CURRENCIES_RANK                    "Rank"
#define CELL_CURRENCIES_PRICE_USD               "PriceUsd"
#define CELL_CURRENCIES_PRICE_BTC               "PriceBtc"
#define CELL_CURRENCIES_VOL24H_USD              "Vol24hUsd"
#define CELL_CURRENCIES_MARKETCAP_USD           "MarketcapUsd"
#define CELL_CURRENCIES_AVAIBLE_SUPPLY          "ACS"
#define CELL_CURRENCIES_TOTAL_SUPPLY            "TCS"
#define CELL_CURRENCIES_MAX_SUPPLY              "MCS"
#define CELL_CURRENCIES_PERCENT_CH_1H           "Per1h"
#define CELL_CURRENCIES_PERCENT_CH_24H          "Per24h"
#define CELL_CURRENCIES_PERCENT_CH_7D           "Per7d"
#define CELL_CURRENCIES_LAST_UPDATE_DATE        "UpdStamp"
// slow updateble cells
#define CELL_CURRENCIES_SL_TYPE                    "Type"
#define CELL_CURRENCIES_SL_MINEABLE                "Mineable"
#define CELL_CURRENCIES_SL_SITE_URLS               "SiteUrls"
#define CELL_CURRENCIES_SL_ANNONCEMENT_URLS        "AnnouncementUrls"
#define CELL_CURRENCIES_SL_CHAT_URLS               "ChatUrls"
#define CELL_CURRENCIES_SL_EXPLORER_URLS           "ExplorerUrls"
#define CELL_CURRENCIES_SL_MSGBOARD_URLS           "MsgboardUrls"
#define CELL_CURRENCIES_SL_SRC_URLS                "SrcUrls"
// cell indexes of the list arg on method to adds values
enum CurrencyFastUpdCellIdx {
    // - for method fast upd:
    IDX_CURRENCIES_NAME = 0,
    IDX_CURRENCIES_SYMBOL,
    IDX_CURRENCIES_RANK,
    IDX_CURRENCIES_PRICE_USD,
    IDX_CURRENCIES_PRICE_BTC,
    IDX_CURRENCIES_VOL24H_USD,
    IDX_CURRENCIES_MARKETCAP_USD,
    IDX_CURRENCIES_AVAIBLE_SUPPLY,
    IDX_CURRENCIES_TOTAL_SUPPLY,
    IDX_CURRENCIES_MAX_SUPPLY,
    IDX_CURRENCIES_PERCENT_CH_1H,
    IDX_CURRENCIES_PERCENT_CH_24H,
    IDX_CURRENCIES_PERCENT_CH_7D,
    IDX_CURRENCIES_LAST_UPDATE_DATE,
    // - for method slow upd:
    IDX_CURRENCIES_SL_TYPE,
    IDX_CURRENCIES_SL_MINEABLE,
    IDX_CURRENCIES_SL_SITE_URLS,
    IDX_CURRENCIES_SL_ANNONCEMENT_URLS,
    IDX_CURRENCIES_SL_CHAT_URLS,
    IDX_CURRENCIES_SL_EXPLORER_URLS,
    IDX_CURRENCIES_SL_MSGBOARD_URLS,
    IDX_CURRENCIES_SL_SRC_URLS,
};
// - for method slow upd:
enum CurrencySlowUpdCellIdx {

};
// current currency type variant
enum CurrencyTypes {
    CURCENCY_TYPE_COIN,
    CURCENCY_TYPE_TOKEN,
};

//////////////////////////////////////////////////////////////////////////
/// Таблица: Проекты - отдельная таблица для ручного заполнения
//////////////////////////////////////////////////////////////////////////
#define T_CURRENCIES_MEM                    "TableCurrenciesMemory"
// - manual edit fields
#define CELL_CURRENCIES_MEM_SYMBOL              "Symbol"
#define CELL_CURRENCIES_MEM_MANUAL_UPD_DATE     "ManualUpdateDate"
#define CELL_CURRENCIES_MEM_APP_AREAS_KEY       "AppAreas_key"
#define CELL_CURRENCIES_MEM_REV_IDEA            "RevolutionIdea"
#define CELL_CURRENCIES_MEM_BASE_PLATFORM_KEY   "BasePlatform_key"
#define CELL_CURRENCIES_MEM_CON_ALG_KEY         "ConsensusAlgorithm_key"
#define CELL_CURRENCIES_MEM_WP_URL              "WhitepaperUrl"
#define CELL_CURRENCIES_MEM_RM_URL              "RoadmapUrl"
#define CELL_CURRENCIES_MEM_F_INTER_COMP        "IsInternationalCompany"
#define CELL_CURRENCIES_MEM_F_WP_IN_CV          "IsWhitepaperInControlVersion"
#define CELL_CURRENCIES_MEM_F_VACANCIES         "IsOpenVacancies"
#define CELL_CURRENCIES_MEM_CONTACTS            "Contacts"          /**
    (example)
    "
    Tel[[>]]+79462634477[[;]]
    Mail[[>]]mail@mail.ru[[;]]
    Address[[>]][[T]]Some text 12345[[T]][[;]]
    Slack[[>]]#namechannel" ...
    **/
// - indexes for method upd:
enum CurrencyMemUpdCellIdx {
    IDX_CURRENCIES_MEM_SYMBOL = 0,
    IDX_CURRENCIES_MEM_MANUAL_UPD_DATE,
    IDX_CURRENCIES_MEM_APP_AREAS_KEY,
    IDX_CURRENCIES_MEM_REV_IDEA,
    IDX_CURRENCIES_MEM_BASE_PLATFORM_KEY,
    IDX_CURRENCIES_MEM_CON_ALG_KEY,
    IDX_CURRENCIES_MEM_WP_URL,
    IDX_CURRENCIES_MEM_RM_URL,
    IDX_CURRENCIES_MEM_F_INTER_COMP,
    IDX_CURRENCIES_MEM_F_WP_IN_CV,
    IDX_CURRENCIES_MEM_F_VACANCIES,
    IDX_CURRENCIES_MEM_CONTACTS,
};

//////////////////////////////////////////////////////////////////////////
/// Таблица: Знаковые даты дорожных карт проектов
//////////////////////////////////////////////////////////////////////////
#define T_ROADMAPSDATESPOOL                 "TableRoadmapDatesPool"
#define CELL_ROADMAPSDATESPOOL_CURRENCY_SYMBOL  "Symbol"
#define CELL_ROADMAPSDATESPOOL_DATE             "Date"
#define CELL_ROADMAPSDATESPOOL_EVENT_INFO       "EventInfo"
// - indexes for method upd:
enum RoadmapDatesUpdCellIdx {
    IDX_ROADMAPSDATESPOOL_CURRENCY_SYMBOL = 0,
    IDX_ROADMAPSDATESPOOL_DATE,
    IDX_ROADMAPSDATESPOOL_EVENT_INFO,
};

//////////////////////////////////////////////////////////////////////////
/// Таблица: Алгоритмы консенсуса
//////////////////////////////////////////////////////////////////////////
#define T_TOKENALG                          "TableTokenAlg"
#define CELL_TOKENALG_NAME                      "Name"
#define CELL_TOKENALG_INFO                      "Info"
// - indexes for method upd:
enum TokenAlgUpdCellIdx {
    IDX_TOKENALG_NAME = 0,
    IDX_TOKENALG_INFO,
};

//////////////////////////////////////////////////////////////////////////
/// Таблица: Область применения проектов (основная)
//////////////////////////////////////////////////////////////////////////
#define T_AREAS                             "TableAppAreas"
#define CELL_AREAS_NAME                         "Name"
// - indexes for method upd:
enum AreasUpdCellIdx {
    IDX_AREAS_NAME = 0,
};

//////////////////////////////////////////////////////////////////////////
/// Таблица: Информация по разрабатываемому проекту
//////////////////////////////////////////////////////////////////////////
#define T_DEVINFO                           "TableDevInfo"
#define CELL_DEVINFO_TEAM_SIZE                  "TeamSize"
#define CELL_DEVINFO_START_DATE                 "StartDate"
#define CELL_DEVINFO_LANGUAGES_USE              "LanguagesUse"
// - indexes for method upd:
enum DevInfoUpdCellIdx {
    IDX_DEVINFO_TEAM_SIZE = 0,
    IDX_DEVINFO_START_DATE,
    IDX_DEVINFO_LANGUAGES_USE,
};

//////////////////////////////////////////////////////////////////////////
/// Таблица: Применяемые техногогии в проектах
//////////////////////////////////////////////////////////////////////////
#define T_TECHPOOL                          "TableTechnologiesThreadsPool"
#define CELL_TECHPOOL_CURRENCY_SYMBOL           "Symbol"
#define CELL_TECHPOOL_TECHNOLOGY_KEY            "Technology_key"
// - indexes for method upd:
enum TechPoolUpdCellIdx {
    IDX_TECHPOOL_CURRENCY_SYMBOL = 0,
    IDX_TECHPOOL_TECHNOLOGY_KEY,
};

//////////////////////////////////////////////////////////////////////////
/// Таблица: Технологии
//////////////////////////////////////////////////////////////////////////
#define T_TECHNOLOGIES                      "TableTechnologies"
#define CELL_TECHNOLOGIES_NAME                  "Name"
#define CELL_TECHNOLOGIES_INFO                  "Info"
// - indexes for method upd:
enum TechnologiesUpdCellIdx {
    IDX_TECHNOLOGIES_NAME = 0,
    IDX_TECHNOLOGIES_INFO,
};

//////////////////////////////////////////////////////////////////////////
/// Таблица: Оценочные параметры по GitHub
//////////////////////////////////////////////////////////////////////////
#define T_GITHUBPOOL                       "TableGithubHistoryPool"
#define CELL_GITHUBPOOL_CURRENCY_SYMBOL         "Symbol"
#define CELL_GITHUBPOOL_PROJECT_NAME            "ProjectName"
#define CELL_GITHUBPOOL_LANGUAGE                "Language"
#define CELL_GITHUBPOOL_LICENSE                 "License"
#define CELL_GITHUBPOOL_COMMITS_NUM             "CommitsNum"
#define CELL_GITHUBPOOL_BRANCHES_NUM            "BranchesNum"
#define CELL_GITHUBPOOL_RELEASES_NUM            "ReleasesNum"
#define CELL_GITHUBPOOL_CONTRIBUTORS_NUM        "ContributorsNum"
#define CELL_GITHUBPOOL_ISSUSES_OPEN_NUM        "IssusesOpenNum"
#define CELL_GITHUBPOOL_ISSUSES_CLOSED_NUM      "IssusesClosedNum"
#define CELL_GITHUBPOOL_PULLREQ_OPEN_NUM        "PullRequestOpenNum"
#define CELL_GITHUBPOOL_PULLREQ_CLOSED_NUM      "PullRequestClosedNum"
#define CELL_GITHUBPOOL_STARS_NUM               "StarsNum"
#define CELL_GITHUBPOOL_FORKS_NUM               "ForksNum"
// - indexes for method upd:
enum GithubPoolUpdCellIdx {
    IDX_GITHUBPOOL_CURRENCY_SYMBOL = 0,
    IDX_GITHUBPOOL_PROJECT_NAME,
    IDX_GITHUBPOOL_LANGUAGE,
    IDX_GITHUBPOOL_LICENSE,
    IDX_GITHUBPOOL_COMMITS_NUM,
    IDX_GITHUBPOOL_BRANCHES_NUM,
    IDX_GITHUBPOOL_RELEASES_NUM,
    IDX_GITHUBPOOL_CONTRIBUTORS_NUM,
    IDX_GITHUBPOOL_ISSUSES_OPEN_NUM,
    IDX_GITHUBPOOL_ISSUSES_CLOSED_NUM,
    IDX_GITHUBPOOL_PULLREQ_OPEN_NUM,
    IDX_GITHUBPOOL_PULLREQ_CLOSED_NUM,
    IDX_GITHUBPOOL_STARS_NUM,
    IDX_GITHUBPOOL_FORKS_NUM,
};

//////////////////////////////////////////////////////////////////////////
/// Таблица: Оценка по биржам
//////////////////////////////////////////////////////////////////////////
#define T_MARKETSPOOL                       "TableMarkets"
#define CELL_MARKETSPOOL_CURRENCY_SYMBOL        "Symbol"
#define CELL_MARKETSPOOL_MARKET                 "Market"
#define CELL_MARKETSPOOL_PAIR                   "Pair"
#define CELL_MARKETSPOOL_VOL_24                 "Volume24"
#define CELL_MARKETSPOOL_VOL_PERC               "VolumePerc"
#define CELL_MARKETSPOOL_PRICE_USD              "PriceUsd"
#define CELL_MARKETSPOOL_PRICE_BTC              "PriceBtc"
// - indexes for method upd:
enum MarketPoolUpdCellIdx {
    IDX_MARKETSPOOL_CURRENCY_SYMBOL = 0,
    IDX_MARKETSPOOL_MARKET,
    IDX_MARKETSPOOL_PAIR,
    IDX_MARKETSPOOL_VOL_24,
    IDX_MARKETSPOOL_VOL_PERC,
    IDX_MARKETSPOOL_PRICE_USD,
    IDX_MARKETSPOOL_PRICE_BTC,
};

#endif // DATABASE_NAMES_H
