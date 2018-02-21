#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{
#ifdef Q_OS_UNIX
    base_path.setPath(QDir::homePath()+QDir::separator()+".apico");
#else
    base_path.setPath(QDir::homePath()+QDir::separator()+"Apico");
#endif
}

/* Методы для подключения к базе данных
 * */
void DataBase::connectToDataBase()
{
    /* Перед подключением к базе данных производим проверку на её существование.
     * В зависимости от результата производим открытие базы данных или её восстановление
     * */
    if(!QFile(base_path.path() + QDir::separator() + DATABASE_NAME).exists()) {
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
}

/* Методы для вставки записей в таблицы
 * */
bool DataBase::inserIntoCurrenciesTableParseInfo(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " T_CURRENCIES " ( " T_CURRENCIES_NAME ", "
                  T_CURRENCIES_SYMBOL ", "
                  T_CURRENCIES_CALC_SUPPLY ", "
                  T_CURRENCIES_VOLUME_USD ", "
                  T_CURRENCIES_VOLUME_BTC ", "
                  T_CURRENCIES_PRICE_USD ", "
                  T_CURRENCIES_PRICE_BTC ", "
                  T_CURRENCIES_MINEABLE " ) "
                  "VALUES (:T_CURRENCIES_NAME, "
                                        ":T_CURRENCIES_SYMBOL, "
                                        ":T_CURRENCIES_CALC_SUPPLY, "
                                        ":T_CURRENCIES_VOLUME_USD, "
                                        ":T_CURRENCIES_VOLUME_BTC, "
                                        ":T_CURRENCIES_PRICE_USD, "
                                        ":T_CURRENCIES_PRICE_BTC, "
                                        ":T_CURRENCIES_MINEABLE)");
    query.bindValue(":T_CURRENCIES_NAME",           data[0].toString());
    query.bindValue(":T_CURRENCIES_SYMBOL",         data[1].toString());
    query.bindValue(":T_CURRENCIES_CALC_SUPPLY",    data[2].toULongLong());
    query.bindValue(":T_CURRENCIES_VOLUME_USD",     data[3].toULongLong());
    query.bindValue(":T_CURRENCIES_VOLUME_BTC",     data[4].toULongLong());
    query.bindValue(":T_CURRENCIES_PRICE_USD",      data[5].toUInt());
    query.bindValue(":T_CURRENCIES_PRICE_BTC",      data[6].toUInt());
    query.bindValue(":T_CURRENCIES_MINEABLE",       data[7].toBool());
    if(!query.exec()) {
        qDebug() << "error insert into " << T_CURRENCIES;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::inserIntoCurrenciesTableManualEditContentInfo(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " T_CURRENCIES " ( " T_CURRENCIES_MANUAL_UPD_DATE ", "
                  T_CURRENCIES_APP_AREAS_ID ", "
                  T_CURRENCIES_REV_IDEA ", "
                  T_CURRENCIES_BASE_PLATFORM_ID ", "
                  T_CURRENCIES_CON_ALG_ID ", "
                  T_CURRENCIES_IS_INTER_COMP ", "
                  T_CURRENCIES_IS_WP_IN_CV ", "
                  T_CURRENCIES_IS_OP_VACANCIES ", "
                  T_CURRENCIES_WP_URL ", "
                  T_CURRENCIES_RM_URL ", "
                  T_CURRENCIES_SITE_URL " ) "
                  "VALUES (:T_CURRENCIES_MANUAL_UPD_DATE, "
                                        ":T_CURRENCIES_APP_AREAS_ID, "
                                        ":T_CURRENCIES_REV_IDEA, "
                                        ":T_CURRENCIES_BASE_PLATFORM_ID, "
                                        ":T_CURRENCIES_CON_ALG_ID, "
                                        ":T_CURRENCIES_IS_INTER_COMP, "
                                        ":T_CURRENCIES_IS_WP_IN_CV, "
                                        ":T_CURRENCIES_IS_OP_VACANCIES, "
                                        ":T_CURRENCIES_WP_URL, "
                                        ":T_CURRENCIES_RM_URL, "
                                        ":T_CURRENCIES_SITE_URL)");
    query.bindValue(":T_CURRENCIES_MANUAL_UPD_DATE",    data[0].toDate());
    query.bindValue(":T_CURRENCIES_APP_AREAS_ID",       data[1].toInt());
    query.bindValue(":T_CURRENCIES_REV_IDEA",           data[2].toString());
    query.bindValue(":T_CURRENCIES_BASE_PLATFORM_ID",   data[3].toInt());
    query.bindValue(":T_CURRENCIES_CON_ALG_ID",         data[4].toInt());
    query.bindValue(":T_CURRENCIES_IS_INTER_COMP",      data[5].toBool());
    query.bindValue(":T_CURRENCIES_IS_WP_IN_CV",        data[6].toBool());
    query.bindValue(":T_CURRENCIES_IS_OP_VACANCIES",    data[7].toBool());
    query.bindValue(":T_CURRENCIES_WP_URL",             data[8].toString());
    query.bindValue(":T_CURRENCIES_RM_URL",             data[9].toString());
    query.bindValue(":T_CURRENCIES_SITE_URL",           data[10].toString());
    if(!query.exec()) {
        qDebug() << "error insert into " << T_CURRENCIES << " : CONTENT_FIELDS";;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::inserIntoCurrenciesTableManualEditContactInfo(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " T_CURRENCIES " ( " T_CURRENCIES_CONTACTS_TEL ", "
                  T_CURRENCIES_CONTACTS_MAIL ", "
                  T_CURRENCIES_CONTACTS_ADDRESS ", "
                  T_CURRENCIES_CONTACTS_SLACK ", "
                  T_CURRENCIES_CONTACTS_TELEGRAM " ) "
                  "VALUES (:T_CURRENCIES_CONTACTS_TEL, "
                                        ":T_CURRENCIES_CONTACTS_MAIL, "
                                        ":T_CURRENCIES_CONTACTS_ADDRESS, "
                                        ":T_CURRENCIES_CONTACTS_SLACK, "
                                        ":T_CURRENCIES_CONTACTS_TELEGRAM)");
    query.bindValue(":T_CURRENCIES_CONTACTS_TEL",       data[0].toString());
    query.bindValue(":T_CURRENCIES_CONTACTS_MAIL",      data[1].toString());
    query.bindValue(":T_CURRENCIES_CONTACTS_ADDRESS",   data[2].toString());
    query.bindValue(":T_CURRENCIES_CONTACTS_SLACK",     data[3].toString());
    query.bindValue(":T_CURRENCIES_CONTACTS_TELEGRAM",  data[4].toString());
    if(!query.exec()) {
        qDebug() << "error insert into " << T_CURRENCIES << " : CONTACTS_FIELDS";
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::inserIntoRoadmapDatesPoolTable(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " T_ROADMAPSDATESPOOL " ( "
                  T_ROADMAPSDATESPOOL_CURRENCY_ID ", "
                  T_ROADMAPSDATESPOOL_DATE ", "
                  T_ROADMAPSDATESPOOL_EVENT_INFO " ) "
                  "VALUES (:T_ROADMAPSDATESPOOL_CURRENCY_ID, "
                                        ":T_ROADMAPSDATESPOOL_DATE, "
                                        ":T_ROADMAPSDATESPOOL_EVENT_INFO)");
    query.bindValue(":T_ROADMAPSDATESPOOL_CURRENCY_ID", data[0].toInt());
    query.bindValue(":T_ROADMAPSDATESPOOL_DATE",        data[1].toDate());
    query.bindValue(":T_ROADMAPSDATESPOOL_EVENT_INFO",  data[2].toString());
    if(!query.exec()) {
        qDebug() << "error insert into " << T_ROADMAPSDATESPOOL;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::inserIntoTokenAlgTable(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " T_TOKENALG " ( " T_TOKENALG_NAME ", "
                  T_TOKENALG_INFO " ) "
                  "VALUES (:T_TOKENALG_NAME, "
                                        ":T_TOKENALG_INFO)");
    query.bindValue(":T_TOKENALG_NAME",     data[0].toString());
    query.bindValue(":T_TOKENALG_INFO",     data[1].toString());
    if(!query.exec()) {
        qDebug() << "error insert into " << T_TOKENALG;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::inserIntoAppAreasTable(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " T_AREAS " ( " T_AREAS_NAME " ) "
                  "VALUES (:T_AREAS_NAME)");
    query.bindValue(":T_AREAS_NAME",     data[0].toString());
    if(!query.exec()) {
        qDebug() << "error insert into " << T_AREAS;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::inserIntoTechnologiesThreadsPoolTable(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " T_TECHPOOL " ( " T_TECHPOOL_CURRENCY_ID ", "
                  T_TECHPOOL_TECHNOLOGY_ID " ) "
                  "VALUES (:T_TECHPOOL_CURRENCY_ID, "
                                        ":T_TECHPOOL_TECHNOLOGY_ID)");
    query.bindValue(":T_TECHPOOL_CURRENCY_ID",      data[0].toInt());
    query.bindValue(":T_TECHPOOL_TECHNOLOGY_ID",    data[1].toInt());
    if(!query.exec()) {
        qDebug() << "error insert into " << T_TECHPOOL;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::inserIntoTechnologiesTable(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " T_TECHNOLOGIES " ( " T_TECHNOLOGIES_NAME ", "
                  T_TECHNOLOGIES_INFO " ) "
                  "VALUES (:T_TECHNOLOGIES_NAME, "
                                        ":T_TECHNOLOGIES_INFO)");
    query.bindValue(":T_TECHNOLOGIES_NAME",     data[0].toString());
    query.bindValue(":T_TECHNOLOGIES_INFO",     data[1].toString());
    if(!query.exec()) {
        qDebug() << "error insert into " << T_TECHNOLOGIES;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::inserIntoGithubHistoryPoolTable(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " T_GITHUBPOOL " ( " T_GITHUBPOOL_CURRENCY_ID ", "
                  T_GITHUBPOOL_PROJECT_NAME ", "
                  T_GITHUBPOOL_LANGUAGE ", "
                  T_GITHUBPOOL_LICENSE ", "
                  T_GITHUBPOOL_COMMITS_NUM ", "
                  T_GITHUBPOOL_BRANCHES_NUM ", "
                  T_GITHUBPOOL_RELEASES_NUM ", "
                  T_GITHUBPOOL_CONTRIBUTORS_NUM ", "
                  T_GITHUBPOOL_ISSUSES_OPEN_NUM ", "
                  T_GITHUBPOOL_ISSUSES_CLOSED_NUM ", "
                  T_GITHUBPOOL_PULLREQ_OPEN_NUM ", "
                  T_GITHUBPOOL_PULLREQ_CLOSED_NUM ", "
                  T_GITHUBPOOL_STARS_NUM ", "
                  T_GITHUBPOOL_FORKS_NUM " ) "
                  "VALUES (:T_GITHUBPOOL_CURRENCY_ID, "
                                        ":T_GITHUBPOOL_PROJECT_NAME, "
                                        ":T_GITHUBPOOL_LANGUAGE, "
                                        ":T_GITHUBPOOL_LICENSE, "
                                        ":T_GITHUBPOOL_COMMITS_NUM, "
                                        ":T_GITHUBPOOL_BRANCHES_NUM, "
                                        ":T_GITHUBPOOL_RELEASES_NUM, "
                                        ":T_GITHUBPOOL_CONTRIBUTORS_NUM, "
                                        ":T_GITHUBPOOL_ISSUSES_OPEN_NUM, "
                                        ":T_GITHUBPOOL_ISSUSES_CLOSED_NUM, "
                                        ":T_GITHUBPOOL_PULLREQ_OPEN_NUM, "
                                        ":T_GITHUBPOOL_PULLREQ_CLOSED_NUM, "
                                        ":T_GITHUBPOOL_STARS_NUM, "
                                        ":T_GITHUBPOOL_FORKS_NUM)");
    query.bindValue(":T_GITHUBPOOL_CURRENCY_ID",        data[0].toInt());
    query.bindValue(":T_GITHUBPOOL_PROJECT_NAME",       data[1].toString());
    query.bindValue(":T_GITHUBPOOL_LANGUAGE",           data[2].toString());
    query.bindValue(":T_GITHUBPOOL_LICENSE",            data[3].toString());
    query.bindValue(":T_GITHUBPOOL_COMMITS_NUM",        data[4].toInt());
    query.bindValue(":T_GITHUBPOOL_BRANCHES_NUM",       data[5].toInt());
    query.bindValue(":T_GITHUBPOOL_RELEASES_NUM",       data[6].toInt());
    query.bindValue(":T_GITHUBPOOL_CONTRIBUTORS_NUM",   data[7].toInt());
    query.bindValue(":T_GITHUBPOOL_ISSUSES_OPEN_NUM",   data[8].toInt());
    query.bindValue(":T_GITHUBPOOL_ISSUSES_CLOSED_NUM", data[9].toInt());
    query.bindValue(":T_GITHUBPOOL_PULLREQ_OPEN_NUM",   data[10].toInt());
    query.bindValue(":T_GITHUBPOOL_PULLREQ_CLOSED_NUM", data[10].toInt());
    query.bindValue(":T_GITHUBPOOL_STARS_NUM",          data[10].toInt());
    query.bindValue(":T_GITHUBPOOL_FORKS_NUM",          data[10].toInt());
    if(!query.exec()) {
        qDebug() << "error insert into " << T_GITHUBPOOL;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::inserIntoMarketsTable(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " T_MARKETSPOOL " ( " T_MARKETSPOOL_CURRENCY_ID ", "
                  T_MARKETSPOOL_MARKET ", "
                  T_MARKETSPOOL_PAIR ", "
                  T_MARKETSPOOL_VOL_24 ", "
                  T_MARKETSPOOL_VOL_P ", "
                  T_MARKETSPOOL_PRICE_USD ", "
                  T_MARKETSPOOL_PRICE_BTC " ) "
                  "VALUES (:T_MARKETSPOOL_CURRENCY_ID, "
                                        ":T_MARKETSPOOL_MARKET, "
                                        ":T_MARKETSPOOL_PAIR, "
                                        ":T_MARKETSPOOL_VOL_24, "
                                        ":T_MARKETSPOOL_VOL_P, "
                                        ":T_MARKETSPOOL_PRICE_USD, "
                                        ":T_MARKETSPOOL_PRICE_BTC)");
    query.bindValue(":T_MARKETSPOOL_CURRENCY_ID",       data[0].toInt());
    query.bindValue(":T_MARKETSPOOL_MARKET",            data[1].toString());
    query.bindValue(":T_MARKETSPOOL_PAIR",              data[2].toString());
    query.bindValue(":T_MARKETSPOOL_VOL_24",            data[3].toInt());
    query.bindValue(":T_MARKETSPOOL_VOL_P",             data[4].toInt());
    query.bindValue(":T_MARKETSPOOL_PRICE_USD",         data[5].toInt());
    query.bindValue(":T_MARKETSPOOL_PRICE_BTC",         data[6].toInt());
    if(!query.exec()) {
        qDebug() << "error insert into " << T_MARKETSPOOL;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

/* Метод для открытия базы данных
 * */
bool DataBase::openDataBase()
{
    /* База данных открывается по заданному пути
     * и имени базы данных, если она существует
     * */
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName(base_path.path() + QDir::separator() + DATABASE_NAME);
    qDebug() << base_path.path() + QDir::separator() + DATABASE_NAME;
    if(db.open()) {
        return true;
    } else {
        return false;
    }
}

/* Методы восстановления базы данных
 * */
bool DataBase::restoreDataBase()
{
    if(this->openDataBase()) {
        if(!createAllTables()) {
            return false;
        } else {
            return true;
        }
    } else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
    return false;
}

/* Методы закрытия базы данных
 * */
void DataBase::closeDataBase()
{
    db.close();
}

/* Методы для создания таблиц в базе данных
 * */
bool DataBase::createAllTables()
{
    if((!this->createCurrenciesTable())
            || (!this->createAppAreasTable())
            || (!this->createDevInfoTable())
            || (!this->createGithubHistoryPoolTable())
            || (!this->createMarketsTable())
            || (!this->createRoadmapDatesPoolTable())
            || (!this->createTechnologiesTable())
            || (!this->createTechnologiesThreadsPoolTable())
            ) {
        return false;
    }
    return true;
}

bool DataBase::createCurrenciesTable()
{    
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " T_CURRENCIES " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "                // 0
                            T_CURRENCIES_NAME " VARCHAR(255) NOT NULL,"             // 1
                            T_CURRENCIES_SYMBOL " VARCHAR(10) NOT NULL,"            // 2
                            T_CURRENCIES_CALC_SUPPLY " UNSIGNED BIG INT NOT NULL,"  // 3
                            T_CURRENCIES_VOLUME_USD " UNSIGNED BIG INT NOT NULL,"   // 4
                            T_CURRENCIES_VOLUME_BTC " UNSIGNED BIG INT NOT NULL,"   // 5
                            T_CURRENCIES_PRICE_USD " UNSIGNED INTEGER NOT NULL,"    // 6
                            T_CURRENCIES_PRICE_BTC " UNSIGNED INTEGER NOT NULL,"    // 7
                            T_CURRENCIES_MINEABLE " BOOLEAN NOT NULL"               // 8
                            T_CURRENCIES_MANUAL_UPD_DATE " DATE,"                   // 9
                            T_CURRENCIES_APP_AREAS_ID " INTEGER,"                   // 10
                            T_CURRENCIES_REV_IDEA " TEXT,"                          // 11
                            T_CURRENCIES_BASE_PLATFORM_ID " INTEGER,"               // 12
                            T_CURRENCIES_CON_ALG_ID " INTEGER,"                     // 13
                            T_CURRENCIES_IS_INTER_COMP " BOOLEAN,"                  // 14
                            T_CURRENCIES_IS_WP_IN_CV " BOOLEAN,"                    // 15
                            T_CURRENCIES_IS_OP_VACANCIES " BOOLEAN,"                // 16
                            T_CURRENCIES_WP_URL " TEXT,"                            // 17
                            T_CURRENCIES_RM_URL " TEXT,"                            // 18
                            T_CURRENCIES_SITE_URL " TEXT,"                          // 19
                            T_CURRENCIES_CONTACTS_TEL " VARCHAR(30),"               // 20
                            T_CURRENCIES_CONTACTS_MAIL " VARCHAR(100),"             // 21
                            T_CURRENCIES_CONTACTS_ADDRESS " TEXT,"                  // 22
                            T_CURRENCIES_CONTACTS_SLACK " VARCHAR(100),"            // 23
                            T_CURRENCIES_CONTACTS_TELEGRAM " VARCHAR(100)"          // 24
                        " )"
                    )) {
        qDebug() << "DataBase: error of create " << T_CURRENCIES;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::createRoadmapDatesPoolTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " T_ROADMAPSDATESPOOL " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            T_ROADMAPSDATESPOOL_CURRENCY_ID " INTEGER NOT NULL,"
                            T_ROADMAPSDATESPOOL_DATE " DATE NOT NULL,"
                            T_ROADMAPSDATESPOOL_EVENT_INFO " TEXT"
                        " )"
                    )) {
        qDebug() << "DataBase: error of create " << T_ROADMAPSDATESPOOL;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::createTokenAlgTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " T_TOKENALG " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            T_TOKENALG_NAME " VARCHAR(30) NOT NULL,"
                            T_TOKENALG_INFO " TEXT"
                        " )"
                    )) {
        qDebug() << "DataBase: error of create " << T_TOKENALG;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::createAppAreasTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " T_AREAS " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            T_AREAS_NAME " VARCHAR(50) NOT NULL"
                        " )"
                    )) {
        qDebug() << "DataBase: error of create " << T_AREAS;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::createDevInfoTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " T_DEVINFO " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            T_DEVINFO_TEAM_SIZE " INTEGER NOT NULL,"
                            T_DEVINFO_START_DATE " INTEGER NOT NULL"
                            T_DEVINFO_LANGUAGES_USE " TEXT NOT NULL"
                        " )"
                    )) {
        qDebug() << "DataBase: error of create " << T_DEVINFO;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::createTechnologiesThreadsPoolTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " T_TECHPOOL " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            T_TECHPOOL_CURRENCY_ID " INTEGER NOT NULL,"
                            T_TECHPOOL_TECHNOLOGY_ID " INTEGER NOT NULL"
                        " )"
                    )) {
        qDebug() << "DataBase: error of create " << T_TECHPOOL;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::createTechnologiesTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " T_TECHNOLOGIES " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            T_TECHNOLOGIES_NAME " VARCHAR(50) NOT NULL,"
                            T_TECHNOLOGIES_INFO " TEXT NOT NULL"
                        " )"
                    )) {
        qDebug() << "DataBase: error of create " << T_TECHNOLOGIES;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::createGithubHistoryPoolTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " T_GITHUBPOOL " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            T_GITHUBPOOL_CURRENCY_ID " INTEGER NOT NULL,"
                            T_GITHUBPOOL_PROJECT_NAME " VARCHAR(50) NOT NULL,"
                            T_GITHUBPOOL_LANGUAGE " VARCHAR(20) NOT NULL,"
                            T_GITHUBPOOL_LICENSE " VARCHAR(50) NOT NULL,"
                            T_GITHUBPOOL_COMMITS_NUM " INTEGER NOT NULL,"
                            T_GITHUBPOOL_BRANCHES_NUM " INTEGER NOT NULL,"
                            T_GITHUBPOOL_RELEASES_NUM " INTEGER NOT NULL,"
                            T_GITHUBPOOL_CONTRIBUTORS_NUM " INTEGER NOT NULL,"
                            T_GITHUBPOOL_ISSUSES_OPEN_NUM " INTEGER NOT NULL,"
                            T_GITHUBPOOL_ISSUSES_CLOSED_NUM " INTEGER NOT NULL,"
                            T_GITHUBPOOL_PULLREQ_OPEN_NUM " INTEGER NOT NULL,"
                            T_GITHUBPOOL_PULLREQ_CLOSED_NUM " INTEGER NOT NULL,"
                            T_GITHUBPOOL_STARS_NUM " INTEGER NOT NULL,"
                            T_GITHUBPOOL_FORKS_NUM " INTEGER NOT NULL"
                        " )"
                    )) {
        qDebug() << "DataBase: error of create " << T_GITHUBPOOL;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::createMarketsTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " T_MARKETSPOOL " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            T_MARKETSPOOL_CURRENCY_ID " INTEGER NOT NULL,"
                            T_MARKETSPOOL_MARKET " VARCHAR(20) NOT NULL,"
                            T_MARKETSPOOL_PAIR " VARCHAR(20) NOT NULL,"
                            T_MARKETSPOOL_VOL_24 " UNSIGNED BIG INT NOT NULL,"
                            T_MARKETSPOOL_VOL_P " INTEGER NOT NULL,"
                            T_MARKETSPOOL_PRICE_USD " INTEGER NOT NULL,"
                            T_MARKETSPOOL_PRICE_BTC " INTEGER NOT NULL"
                        " )"
                    )) {
        qDebug() << "DataBase: error of create " << T_MARKETSPOOL;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}
