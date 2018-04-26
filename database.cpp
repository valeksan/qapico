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
bool DataBase::inserIntoCurrenciesTable(const QHash<int,QVariant> &roles)
{
    if(roles.empty()) {
        return false;
    }

    QSqlQuery query;
    QStringList sql_cells;
    QStringList sql_values;
    QString sql_cmd;
    QList<int> keys = roles.keys();
    for(int i = 0, fix_space_cnt = 0; i < keys.size(); i++) {
        int key = keys.at(i);
        switch (key) {
        case IDX_CURRENCIES_NAME:
            sql_cells.append(CELL_CURRENCIES_NAME);
            break;
        case IDX_CURRENCIES_SYMBOL:
            sql_cells.append(CELL_CURRENCIES_SYMBOL);
            break;
        case IDX_CURRENCIES_RANK:
            sql_cells.append(CELL_CURRENCIES_RANK);
            break;
        case IDX_CURRENCIES_PRICE_USD:
            sql_cells.append(CELL_CURRENCIES_PRICE_USD);
            break;
        case IDX_CURRENCIES_PRICE_BTC:
            sql_cells.append(CELL_CURRENCIES_PRICE_BTC);
            break;
        case IDX_CURRENCIES_VOL24H_USD:
            sql_cells.append(CELL_CURRENCIES_VOL24H_USD);
            break;
        case IDX_CURRENCIES_MARKETCAP_USD:
            sql_cells.append(CELL_CURRENCIES_MARKETCAP_USD);
            break;
        case IDX_CURRENCIES_AVAIBLE_SUPPLY:
            sql_cells.append(CELL_CURRENCIES_AVAIBLE_SUPPLY);
            break;
        case IDX_CURRENCIES_TOTAL_SUPPLY:
            sql_cells.append(CELL_CURRENCIES_TOTAL_SUPPLY);
            break;
        case IDX_CURRENCIES_MAX_SUPPLY:
            sql_cells.append(CELL_CURRENCIES_MAX_SUPPLY);
            break;
        case IDX_CURRENCIES_PERCENT_CH_1H:
            sql_cells.append(CELL_CURRENCIES_PERCENT_CH_1H);
            break;
        case IDX_CURRENCIES_PERCENT_CH_24H:
            sql_cells.append(CELL_CURRENCIES_PERCENT_CH_24H);
            break;
        case IDX_CURRENCIES_PERCENT_CH_7D:
            sql_cells.append(CELL_CURRENCIES_PERCENT_CH_7D);
            break;
        case IDX_CURRENCIES_LAST_UPDATE_DATE:
            sql_cells.append(CELL_CURRENCIES_LAST_UPDATE_DATE);
            break;
        case IDX_CURRENCIES_SL_TYPE:
            sql_cells.append(CELL_CURRENCIES_SL_TYPE);
            break;
        case IDX_CURRENCIES_SL_MINEABLE:
            sql_cells.append(CELL_CURRENCIES_SL_MINEABLE);
            break;
        case IDX_CURRENCIES_SL_SITE_URLS:
            sql_cells.append(CELL_CURRENCIES_SL_SITE_URLS);
            break;
        case IDX_CURRENCIES_SL_ANNONCEMENT_URLS:
            sql_cells.append(CELL_CURRENCIES_SL_ANNONCEMENT_URLS);
            break;
        case IDX_CURRENCIES_SL_CHAT_URLS:
            sql_cells.append(CELL_CURRENCIES_SL_CHAT_URLS);
            break;
        case IDX_CURRENCIES_SL_EXPLORER_URLS:
            sql_cells.append(CELL_CURRENCIES_SL_EXPLORER_URLS);
            break;
        case IDX_CURRENCIES_SL_MSGBOARD_URLS:
            sql_cells.append(CELL_CURRENCIES_SL_MSGBOARD_URLS);
            break;
        case IDX_CURRENCIES_SL_SRC_URLS:
            sql_cells.append(CELL_CURRENCIES_SL_SRC_URLS);
            break;
        default:
            break;
        }
        if((fix_space_cnt + sql_cells.size() - 1) == i) {
            sql_values.append(":val_" + key);
        } else {
            ++fix_space_cnt;
        }
    }
    if(sql_cells.empty()) {
        return false;
    }
    sql_cmd = QString("INSERT OR REPLACE INTO " T_CURRENCIES " ( ");
    sql_cmd += sql_cells.join(", ");
    sql_cmd += ") VALUES ( ";
    sql_cmd += sql_values.join(", ");
    sql_cmd += " );";

    query.prepare(sql_cmd);

    for(int i = 0; i < keys.size(); i++) {
        int key = keys.at(i);
        switch (key) {
        case IDX_CURRENCIES_NAME:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_CURRENCIES_SYMBOL:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_CURRENCIES_RANK:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toInt());
            break;
        case IDX_CURRENCIES_PRICE_USD:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toDouble());
            break;
        case IDX_CURRENCIES_PRICE_BTC:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toDouble());
            break;
        case IDX_CURRENCIES_VOL24H_USD:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toDouble());
            break;
        case IDX_CURRENCIES_MARKETCAP_USD:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toDouble());
            break;
        case IDX_CURRENCIES_AVAIBLE_SUPPLY:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toDouble());
            break;
        case IDX_CURRENCIES_TOTAL_SUPPLY:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toDouble());
            break;
        case IDX_CURRENCIES_MAX_SUPPLY:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toDouble());
            break;
        case IDX_CURRENCIES_PERCENT_CH_1H:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toFloat());
            break;
        case IDX_CURRENCIES_PERCENT_CH_24H:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toFloat());
            break;
        case IDX_CURRENCIES_PERCENT_CH_7D:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toFloat());
            break;
        case IDX_CURRENCIES_LAST_UPDATE_DATE:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toLongLong()); // datetime stamp
            break;
        case IDX_CURRENCIES_SL_TYPE:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toInt());
            break;
        case IDX_CURRENCIES_SL_MINEABLE:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toBool());
            break;
        case IDX_CURRENCIES_SL_SITE_URLS:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_CURRENCIES_SL_ANNONCEMENT_URLS:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_CURRENCIES_SL_CHAT_URLS:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_CURRENCIES_SL_EXPLORER_URLS:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_CURRENCIES_SL_MSGBOARD_URLS:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_CURRENCIES_SL_SRC_URLS:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        default:
            break;
        }
    }

    if(!query.exec()) {
        qDebug() << "error insert into " << T_CURRENCIES;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::inserIntoCurrenciesMemTable(const QHash<int, QVariant> &roles)
{
    if(roles.empty()) {
        return false;
    }

    QSqlQuery query;
    QStringList sql_cells;
    QStringList sql_values;
    QString sql_cmd;
    QList<int> keys = roles.keys();
    for(int i = 0, fix_space_cnt = 0; i < keys.size(); i++) {
        int key = keys.at(i);
        switch (key) {
        case IDX_CURRENCIES_MEM_SYMBOL:
            sql_cells.append(CELL_CURRENCIES_MEM_SYMBOL);
            break;
        case IDX_CURRENCIES_MEM_MANUAL_UPD_DATE:
            sql_cells.append(CELL_CURRENCIES_MEM_MANUAL_UPD_DATE);
            break;
        case IDX_CURRENCIES_MEM_APP_AREAS_KEY:
            sql_cells.append(CELL_CURRENCIES_MEM_APP_AREAS_KEY);
            break;
        case IDX_CURRENCIES_MEM_REV_IDEA:
            sql_cells.append(CELL_CURRENCIES_MEM_REV_IDEA);
            break;
        case IDX_CURRENCIES_MEM_BASE_PLATFORM_KEY:
            sql_cells.append(CELL_CURRENCIES_MEM_BASE_PLATFORM_KEY);
            break;
        case IDX_CURRENCIES_MEM_CON_ALG_KEY:
            sql_cells.append(CELL_CURRENCIES_MEM_CON_ALG_KEY);
            break;
        case IDX_CURRENCIES_MEM_WP_URL:
            sql_cells.append(CELL_CURRENCIES_MEM_WP_URL);
            break;
        case IDX_CURRENCIES_MEM_RM_URL:
            sql_cells.append(CELL_CURRENCIES_MEM_RM_URL);
            break;
        case IDX_CURRENCIES_MEM_F_INTER_COMP:
            sql_cells.append(CELL_CURRENCIES_MEM_F_INTER_COMP);
            break;
        case IDX_CURRENCIES_MEM_F_WP_IN_CV:
            sql_cells.append(CELL_CURRENCIES_MEM_F_WP_IN_CV);
            break;
        case IDX_CURRENCIES_MEM_CONTACTS:
            sql_cells.append(CELL_CURRENCIES_MEM_CONTACTS);
            break;
        default:
            break;
        }
        if((fix_space_cnt + sql_cells.size() - 1) == i) {
            sql_values.append(":val_" + key);
        } else {
            ++fix_space_cnt;
        }
    }
    if(sql_cells.empty()) {
        return false;
    }
    sql_cmd = QString("INSERT OR REPLACE INTO " T_CURRENCIES_MEM " ( ");
    sql_cmd += sql_cells.join(", ");
    sql_cmd += ") VALUES ( ";
    sql_cmd += sql_values.join(", ");
    sql_cmd += " );";

    query.prepare(sql_cmd);

    for(int i = 0; i < keys.size(); i++) {
        int key = keys.at(i);
        switch (key) {
        case IDX_CURRENCIES_MEM_SYMBOL:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_CURRENCIES_MEM_MANUAL_UPD_DATE:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toLongLong());
            break;
        case IDX_CURRENCIES_MEM_APP_AREAS_KEY:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_CURRENCIES_MEM_REV_IDEA:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_CURRENCIES_MEM_BASE_PLATFORM_KEY:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_CURRENCIES_MEM_CON_ALG_KEY:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_CURRENCIES_MEM_WP_URL:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_CURRENCIES_MEM_RM_URL:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_CURRENCIES_MEM_F_INTER_COMP:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toBool());
            break;
        case IDX_CURRENCIES_MEM_F_WP_IN_CV:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toBool());
            break;
        case IDX_CURRENCIES_MEM_CONTACTS:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        default:
            break;
        }
    }

    if(!query.exec()) {
        qDebug() << "error insert into " << T_CURRENCIES_MEM;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::inserIntoTokenAlgTable(const QHash<int, QVariant> &roles)
{
    if(roles.empty()) {
        return false;
    }

    QSqlQuery query;
    QStringList sql_cells;
    QStringList sql_values;
    QString sql_cmd;
    QList<int> keys = roles.keys();
    for(int i = 0, fix_space_cnt = 0; i < keys.size(); i++) {
        int key = keys.at(i);
        switch (key) {
        case IDX_TOKENALG_NAME:
            sql_cells.append(CELL_TOKENALG_NAME);
            break;
        case IDX_TOKENALG_INFO:
            sql_cells.append(CELL_TOKENALG_INFO);
            break;
        default:
            break;
        }
        if((fix_space_cnt + sql_cells.size() - 1) == i) {
            sql_values.append(":val_" + key);
        } else {
            ++fix_space_cnt;
        }
    }

    if(sql_cells.empty()) {
        return false;
    }
    sql_cmd = QString("INSERT OR REPLACE INTO " T_TOKENALG " ( ");
    sql_cmd += sql_cells.join(", ");
    sql_cmd += ") VALUES ( ";
    sql_cmd += sql_values.join(", ");
    sql_cmd += " );";

    query.prepare(sql_cmd);

    for(int i = 0; i < keys.size(); i++) {
        int key = keys.at(i);
        switch (key) {
        case IDX_TOKENALG_NAME:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_TOKENALG_INFO:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        default:
            break;
        }
    }

    if(!query.exec()) {
        qDebug() << "error insert into " << T_TOKENALG;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::inserIntoAreasTable(const QHash<int, QVariant> &roles)
{
    if(roles.empty()) {
        return false;
    }

    QSqlQuery query;
    QStringList sql_cells;
    QStringList sql_values;
    QString sql_cmd;
    QList<int> keys = roles.keys();
    for(int i = 0, fix_space_cnt = 0; i < keys.size(); i++) {
        int key = keys.at(i);
        switch (key) {
        case IDX_AREAS_NAME:
            sql_cells.append(CELL_AREAS_NAME);
            break;
        case IDX_AREAS_COMMENT:
            sql_cells.append(CELL_AREAS_COMMENT);
            break;
        default:
            break;
        }
        if((fix_space_cnt + sql_cells.size() - 1) == i) {
            sql_values.append(":val_" + key);
        } else {
            ++fix_space_cnt;
        }
    }

    if(sql_cells.empty()) {
        return false;
    }
    sql_cmd = QString("INSERT OR REPLACE INTO " T_AREAS " ( ");
    sql_cmd += sql_cells.join(", ");
    sql_cmd += ") VALUES ( ";
    sql_cmd += sql_values.join(", ");
    sql_cmd += " );";

    query.prepare(sql_cmd);

    for(int i = 0; i < keys.size(); i++) {
        int key = keys.at(i);
        switch (key) {
        case IDX_AREAS_NAME:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_AREAS_COMMENT:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        default:
            break;
        }
    }
    if(!query.exec()) {
        qDebug() << "error insert into " << T_AREAS;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

/*
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
*/

/*
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
*/

/*
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
*/

/*
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
    query.bindValue(":T_MARKETSPOOL_VOL_24",            data[3].toDouble());
    query.bindValue(":T_MARKETSPOOL_VOL_P",             data[4].toDouble());
    query.bindValue(":T_MARKETSPOOL_PRICE_USD",         data[5].toDouble());
    query.bindValue(":T_MARKETSPOOL_PRICE_BTC",         data[6].toDouble());
    if(!query.exec()) {
        qDebug() << "error insert into " << T_MARKETSPOOL;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}
*/

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

/*
bool DataBase::createCurrenciesTable()
{    
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " T_CURRENCIES " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "                // 0
                            T_CURRENCIES_NAME " VARCHAR(255) NOT NULL,"             // 1
                            T_CURRENCIES_SYMBOL " VARCHAR(10) NOT NULL,"            // 2
                            T_CURRENCIES_CALC_SUPPLY " UNSIGNED BIG INT NOT NULL,"  // 3
                            T_CURRENCIES_VOLUME_USD " REAL NOT NULL,"               // 4
                            T_CURRENCIES_VOLUME_BTC " REAL NOT NULL,"               // 5
                            T_CURRENCIES_PRICE_USD " REAL NOT NULL,"                // 6
                            T_CURRENCIES_PRICE_BTC " REAL NOT NULL,"                // 7
                            T_CURRENCIES_MINEABLE " BOOLEAN NOT NULL,"              // 8
                            T_CURRENCIES_IS_OPENSOURCE_PROJECT " BOOLEAN,"          // 9
                            T_CURRENCIES_MANUAL_UPD_DATE " DATE,"                   // 10
                            T_CURRENCIES_APP_AREAS_ID " INTEGER,"                   // 11
                            T_CURRENCIES_REV_IDEA " TEXT,"                          // 12
                            T_CURRENCIES_BASE_PLATFORM_ID " INTEGER,"               // 13
                            T_CURRENCIES_CON_ALG_ID " INTEGER,"                     // 14
                            T_CURRENCIES_IS_INTER_COMP " BOOLEAN,"                  // 15
                            T_CURRENCIES_IS_WP_IN_CV " BOOLEAN,"                    // 16
                            T_CURRENCIES_IS_OP_VACANCIES " BOOLEAN,"                // 17
                            T_CURRENCIES_WP_URL " TEXT,"                            // 18
                            T_CURRENCIES_RM_URL " TEXT,"                            // 19
                            T_CURRENCIES_SITE_URL " TEXT,"                          // 20
                            T_CURRENCIES_CONTACTS_TEL " VARCHAR(30),"               // 21
                            T_CURRENCIES_CONTACTS_MAIL " VARCHAR(100),"             // 22
                            T_CURRENCIES_CONTACTS_ADDRESS " TEXT,"                  // 23
                            T_CURRENCIES_CONTACTS_SLACK " VARCHAR(100),"            // 24
                            T_CURRENCIES_CONTACTS_TELEGRAM " VARCHAR(100)"          // 25
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
                            T_DEVINFO_START_DATE " INTEGER NOT NULL,"
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
                            T_MARKETSPOOL_VOL_24 " REAL NOT NULL,"
                            T_MARKETSPOOL_VOL_P " REAL NOT NULL,"
                            T_MARKETSPOOL_PRICE_USD " REAL NOT NULL,"
                            T_MARKETSPOOL_PRICE_BTC " REAL NOT NULL"
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
*/
