#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{
#ifdef Q_OS_UNIX
    base_path.setPath(QDir::homePath()+QDir::separator()+".Apico");
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
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            T_CURRENCIES_NAME " VARCHAR(255) NOT NULL,"
                            T_CURRENCIES_SYMBOL " VARCHAR(10) NOT NULL,"
                            T_CURRENCIES_CALC_SUPPLY " UNSIGNED BIG INT NOT NULL,"
                            T_CURRENCIES_VOLUME_USD " UNSIGNED BIG INT NOT NULL,"
                            T_CURRENCIES_VOLUME_BTC " UNSIGNED BIG INT NOT NULL,"
                            T_CURRENCIES_PRICE_USD " INTEGER NOT NULL,"
                            T_CURRENCIES_PRICE_BTC " INTEGER NOT NULL,"
                            T_CURRENCIES_MINEABLE " BOOLEAN NOT NULL"
                            T_CURRENCIES_MANUAL_UPD_DATE " DATE NOT NULL,"
                            T_CURRENCIES_APP_AREAS_ID " INTEGER,"
                            T_CURRENCIES_REV_IDEA " TEXT,"
                            T_CURRENCIES_BASE_PLATFORM_ID " INTEGER,"
                            T_CURRENCIES_CON_ALG_ID " INTEGER,"
                            T_CURRENCIES_IS_INTER_COMP " BOOLEAN,"
                            T_CURRENCIES_IS_WP_IN_CV " BOOLEAN,"
                            T_CURRENCIES_IS_OP_VACANCIES " BOOLEAN,"
                            T_CURRENCIES_WP_URL " TEXT,"
                            T_CURRENCIES_RM_URL " TEXT,"
                            T_CURRENCIES_SITE_URL " TEXT,"
                            T_CURRENCIES_CONTACTS_TEL " VARCHAR(30),"
                            T_CURRENCIES_CONTACTS_MAIL " VARCHAR(100),"
                            T_CURRENCIES_CONTACTS_ADDRESS " TEXT,"
                            T_CURRENCIES_CONTACTS_SLACK " VARCHAR(100),"
                            T_CURRENCIES_CONTACTS_TELEGRAM " VARCHAR(100)"
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
