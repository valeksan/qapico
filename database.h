#ifndef DATABASE_H
#define DATABASE_H

/* examples
 * https://evileg.com/ru/post/67/
 * https://evileg.com/ru/post/62/
*/


#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDir>
#include <QDate>
#include <QDebug>

/* Директивы имен таблицы, полей таблицы и базы данных */
#define DATABASE_HOSTNAME       "ApicoDataBase"
#define DATABASE_NAME           "DataBase.db"

// Таблица: Проекты
#define T_CURRENCIES_ABLE       "TableCurrencies"
#define T_CURRENCIES_NAME       "Name"
#define T_CURRENCIES_SYMBOL     "Symbol"
#define T_CURRENCIES_PRICE_USD  "PriceUsd"
#define T_CURRENCIES_PRICE_BTC  "PriceBtc"
#define T_CURRENCIES_CALC_SUPPLY "CalculatingSupply"
#define T_CURRENCIES_MINEABLE   "Mineable"
#define T_CURRENCIES_VOLUME_USD "VolumeUsd"
#define T_CURRENCIES_VOLUME_BTC "VolumeBtc"

// Таблица: Информация
#define T_INFO                  "TableBaseInfo"
#define T_INFO_UPD_DATE         "DateUpdate"
#define T_INFO_APP_AREAS_ID     "AppAreas_id"
#define T_INFO_REV_IDEA         "RevolutionIdea"
#define T_INFO_BASE_PLATFORM_ID "BasePlatform_id"
#define T_INFO_CON_ALG_ID       "ConsensusAlgorithm_id"
#define T_INFO_IS_INTER_COMP    "IsInternationalCompany"
#define T_INFO_IS_WP_IN_CV      "IsWhitepaperInControlVersion"
#define T_INFO_IS_OP_VACANCIES  "IsOpenVacancies"
#define T_INFO_WP_URL           "WhitepaperUrl"
#define T_INFO_RM_URL           "RoadmapUrl"
#define T_INFO_SITE_URL         "SiteUrl"

// Таблица: Знаковые даты дорожных карт проектов
#define T_ROADMAPSDATESPOOL                 "TableRoadmapDatesPool"
#define T_ROADMAPSDATESPOOL_DATE            "Date"
#define T_ROADMAPSDATESPOOL_EVENT_INFO      "EventInfo"

// Таблица: Алгоритмы консенсуса
#define T_TOKENALG              "TableTokenAlg"
#define T_TOKENALG_NAME         "Name"
#define T_TOKENALG_INFO         "Info"

// Таблица: Область применения проектов (основная)
#define T_AREAS                 "TableAppAreas"
#define T_AREAS_NAME            "Name"

// Таблица: Информация по разрабатываемому проекту
#define T_DEVINFO               "TableDevInfo"
#define T_DEVINFO_TEAM_SIZE     "TeamSize"
#define T_DEVINFO_START_DATE    "StartDate"
#define T_DEVINFO_LANGUAGES_USE "LanguagesUse"

// Таблица: Применяемые техногогии в проектах
#define T_TECHPOOL                          "TableTechnologiesThreadsPool"
#define T_TECHPOOL_CURRENCY_ID              "Currency_id"
#define T_TECHPOOL_TECHNOLOGY_ID            "Technology_id"

// Таблица: Технологии
#define T_TECHNOLOGIES                      "TableTechnologies"
#define T_TECHNOLOGIES_NAME                 "Name"
#define T_TECHNOLOGIES_INFO                 "Info"

// Таблица: Оценочные параметры по GitHub
#define T_GITHUBPOOL                       "TableGithubHistoryPool"
#define T_GITHUBPOOL_CURRENCY_ID           "Currency_id"
#define T_GITHUBPOOL_LANGUAGE              "Language"
#define T_GITHUBPOOL_PROJECT_NAME          "ProjectName"
#define T_GITHUBPOOL_COMMITS_NUM           "CommitsNum"
#define T_GITHUBPOOL_BRANCHES_NUM          "BranchesNum"
#define T_GITHUBPOOL_RELEASES_NUM          "ReleasesNum"
#define T_GITHUBPOOL_CONTRIBUTORS_NUM      "ContributorsNum"
#define T_GITHUBPOOL_LICENSE               "License"
#define T_GITHUBPOOL_ISSUSES_OPEN_NUM      "IssusesOpenNum"
#define T_GITHUBPOOL_ISSUSES_CLOSED_NUM    "IssusesClosedNum"
#define T_GITHUBPOOL_PULLREQ_OPEN_NUM      "PullRequestOpenNum"
#define T_GITHUBPOOL_PULLREQ_CLOSED_NUM    "PullRequestClosedNum"
#define T_GITHUBPOOL_STARS_NUM             "StarsNum"
#define T_GITHUBPOOL_FORKS_NUM             "ForksNum"

// Таблица: Контакты
#define T_CONTACTS              "TableContacts"
#define T_CONTACTS_TEL          "Phone"
#define T_CONTACTS_MAIL         "Mail"
#define T_CONTACTS_ADDRESS      "Address"
#define T_CONTACTS_SLACK        "Slack"
#define T_CONTACTS_TELEGRAM     "Telegram"

// Таблица: Оценка по биржам
#define T_MARKETSPOOL                       "TableMarkets"
#define T_MARKETSPOOL_CURRENCY_ID           "Currency_id"
#define T_MARKETSPOOL_MARKET                "Market"
#define T_MARKETSPOOL_PAIR                  "Pair"
#define T_MARKETSPOOL_VOL_24                "Volume24"
#define T_MARKETSPOOL_VOL_P                 "VolumeP"
#define T_MARKETSPOOL_PRICE_USD             "PriceUsd"
#define T_MARKETSPOOL_PRICE_BTC             "PriceBtc"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
    /* Методы для непосредственной работы с классом
     * Подключение к базе данных и вставка записей в таблицу
     * */
    void connectToDataBase();
    bool inserIntoCurrenciesTable(const QVariantList &data);
    bool inserIntoInfoTable(const QVariantList &data);
    bool inserIntoRoadmapDatesPoolTable(const QVariantList &data);
    bool inserIntoTokenAlgTable(const QVariantList &data);
    bool inserIntoAppAreasTable(const QVariantList &data);
    bool inserIntoDevInfoTable(const QVariantList &data);
    bool inserIntoTechnologiesThreadsPoolTable(const QVariantList &data);
    bool inserIntoTechnologiesTable(const QVariantList &data);
    bool inserIntoGithubHistoryPoolTable(const QVariantList &data);
    bool inserIntoContactsTable(const QVariantList &data);
    bool inserIntoMarketsTable(const QVariantList &data);

private:
    // Сам объект базы данных, с которым будет производиться работа
    QSqlDatabase    db;
    QDir            base_path;

private:
    /* Внутренние методы для работы с базой данных
     * */
    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createAllTables();
    bool createCurrenciesTable();
    bool createInfoTable();
    bool createRoadmapDatesPoolTable();
    bool createTokenAlgTable();
    bool createAppAreasTable();
    bool createDevInfoTable();
    bool createTechnologiesThreadsPoolTable();
    bool createTechnologiesTable();
    bool createGithubHistoryPoolTable();
    bool createContactsTable();
    bool createMarketsTable();

signals:

public slots:
};

#endif // DATABASE_H
