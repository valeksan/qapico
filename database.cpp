#include "database.h"

DataBase::DataBase(QObject *parent) :
    QObject(parent)
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
    if(!QFile(getDatabaseFilename()/*base_path.path() + QDir::separator() + DATABASE_NAME*/).exists()) {
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
}

/* Методы для вставки записей в таблицы
 * */
bool DataBase::insertIntoCurrenciesTable(const QHash<int,QVariant> &roles, int table_idx, bool orReplace)
{
    if(roles.empty()) {
        return false;
    }

    if(!createCurrenciesTable(table_idx, true)) {
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
        case IDX_CURRENCIES_ID:
            sql_cells.append(CELL_CURRENCIES_ID);
            break;
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
        case IDX_CURRENCIES_CMC_PAGE_URL:
            sql_cells.append(CELL_CURRENCIES_CMC_PAGE_URL);
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
            sql_values.append(QString(":val_%1").arg(key));
        } else {
            ++fix_space_cnt;
        }
    }
    if(sql_cells.empty()) {
        return false;
    }
    sql_cmd = QString("INSERT%1 INTO %2 ( ").arg((orReplace?" OR REPLACE":"")).arg(getTableNameByIdx(table_idx));
    sql_cmd += sql_cells.join(", ");
    sql_cmd += ") VALUES ( ";
    sql_cmd += sql_values.join(", ");
    sql_cmd += " );";

    query.prepare(sql_cmd);

    //qDebug() << "prepare: " << sql_cmd;

    for(int i = 0; i < keys.size(); i++) {
        int key = keys.at(i);
        switch (key) {
        case IDX_CURRENCIES_ID:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
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
        case IDX_CURRENCIES_CMC_PAGE_URL:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
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
        qDebug() << "error insert into " << getTableNameByIdx(table_idx);
        qDebug() << query.lastError().text();
        qDebug() << query.executedQuery();
        qDebug() << "Name: " << roles.value(IDX_CURRENCIES_NAME).toString();
        qDebug() << "Symbol: " << roles.value(IDX_CURRENCIES_SYMBOL).toString();
        qDebug() << "Rank: " << roles.value(IDX_CURRENCIES_RANK).toInt();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::insertIntoCurrenciesMemTable(const QHash<int, QVariant> &roles)
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
        case IDX_CURRENCIES_MEM_ID:
            sql_cells.append(CELL_CURRENCIES_MEM_ID);
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
        case IDX_CURRENCIES_MEM_ID:
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

bool DataBase::insertIntoConsensusAlgTable(const QHash<int, QVariant> &roles)
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
        case IDX_CONSENSUSALG_NAME:
            sql_cells.append(CELL_CONSENSUSALG_NAME);
            break;
        case IDX_CONSENSUSALG_INFO:
            sql_cells.append(CELL_CONSENSUSALG_INFO);
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
    sql_cmd = QString("INSERT OR REPLACE INTO " T_CONSENSUSALG " ( ");
    sql_cmd += sql_cells.join(", ");
    sql_cmd += ") VALUES ( ";
    sql_cmd += sql_values.join(", ");
    sql_cmd += " );";

    query.prepare(sql_cmd);

    for(int i = 0; i < keys.size(); i++) {
        int key = keys.at(i);
        switch (key) {
        case IDX_CONSENSUSALG_NAME:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_CONSENSUSALG_INFO:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        default:
            break;
        }
    }

    if(!query.exec()) {
        qDebug() << "error insert into " << T_CONSENSUSALG;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::insertIntoAreasTable(const QHash<int, QVariant> &roles)
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

bool DataBase::insertIntoDevInfoTable(const QHash<int, QVariant> &roles)
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
        case IDX_DEVINFO_CURRENCY_ID:
            sql_cells.append(CELL_DEVINFO_CURRENCY_ID);
            break;
        case IDX_DEVINFO_PROJECT_NAME:
            sql_cells.append(CELL_DEVINFO_PROJECT_NAME);
            break;
        case IDX_DEVINFO_START_DATETIME_STR:
            sql_cells.append(CELL_DEVINFO_START_DATETIME_STR);
            break;
        case IDX_DEVINFO_LANGUAGE_USE:
            sql_cells.append(CELL_DEVINFO_LANGUAGE_USE);
            break;
        case IDX_DEVINFO_LICENSE:
            sql_cells.append(CELL_DEVINFO_LICENSE);
            break;
        case IDX_DEVINFO_F_IS_FORK:
            sql_cells.append(CELL_DEVINFO_F_IS_FORK);
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
    sql_cmd = QString("INSERT OR REPLACE INTO " T_DEVINFO " ( ");
    sql_cmd += sql_cells.join(", ");
    sql_cmd += ") VALUES ( ";
    sql_cmd += sql_values.join(", ");
    sql_cmd += " );";

    query.prepare(sql_cmd);

    for(int i = 0; i < keys.size(); i++) {
        int key = keys.at(i);
        switch (key) {
        case IDX_DEVINFO_CURRENCY_ID:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_DEVINFO_PROJECT_NAME:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_DEVINFO_START_DATETIME_STR:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_DEVINFO_LANGUAGE_USE:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_DEVINFO_LICENSE:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_DEVINFO_F_IS_FORK:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toBool());
            break;
        default:
            break;
        }
    }
    if(!query.exec()) {
        qDebug() << "error insert into " << T_DEVINFO;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::insertIntoGithubHistoryPoolTable(const QHash<int, QVariant> &roles)
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
        case IDX_GITHUBPOOL_CURRENCY_ID:
            sql_cells.append(CELL_GITHUBPOOL_CURRENCY_ID);
            break;
        case IDX_GITHUBPOOL_PROJECT_NAME:
            sql_cells.append(CELL_GITHUBPOOL_PROJECT_NAME);
            break;
        case IDX_GITHUBPOOL_COMMITS_NUM:
            sql_cells.append(CELL_GITHUBPOOL_COMMITS_NUM);
            break;
        case IDX_GITHUBPOOL_BRANCHES_NUM:
            sql_cells.append(CELL_GITHUBPOOL_BRANCHES_NUM);
            break;
        case IDX_GITHUBPOOL_RELEASES_NUM:
            sql_cells.append(CELL_GITHUBPOOL_RELEASES_NUM);
            break;
        case IDX_GITHUBPOOL_CONTRIBUTORS_NUM:
            sql_cells.append(CELL_GITHUBPOOL_CONTRIBUTORS_NUM);
            break;
        case IDX_GITHUBPOOL_ISSUSES_OPEN_NUM:
            sql_cells.append(CELL_GITHUBPOOL_ISSUSES_OPEN_NUM);
            break;
        case IDX_GITHUBPOOL_ISSUSES_CLOSED_NUM:
            sql_cells.append(CELL_GITHUBPOOL_ISSUSES_CLOSED_NUM);
            break;
        case IDX_GITHUBPOOL_PULLREQ_OPEN_NUM:
            sql_cells.append(CELL_GITHUBPOOL_PULLREQ_OPEN_NUM);
            break;
        case IDX_GITHUBPOOL_PULLREQ_CLOSED_NUM:
            sql_cells.append(CELL_GITHUBPOOL_PULLREQ_CLOSED_NUM);
            break;
        case IDX_GITHUBPOOL_STARS_NUM:
            sql_cells.append(CELL_GITHUBPOOL_STARS_NUM);
            break;
        case IDX_GITHUBPOOL_FORKS_NUM:
            sql_cells.append(CELL_GITHUBPOOL_FORKS_NUM);
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
    sql_cmd = QString("INSERT OR REPLACE INTO " T_GITHUBPOOL " ( ");
    sql_cmd += sql_cells.join(", ");
    sql_cmd += ") VALUES ( ";
    sql_cmd += sql_values.join(", ");
    sql_cmd += " );";

    query.prepare(sql_cmd);

    for(int i = 0; i < keys.size(); i++) {
        int key = keys.at(i);
        switch (key) {
        case IDX_GITHUBPOOL_CURRENCY_ID:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_GITHUBPOOL_PROJECT_NAME:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_GITHUBPOOL_COMMITS_NUM:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toUInt());
            break;
        case IDX_GITHUBPOOL_BRANCHES_NUM:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toUInt());
            break;
        case IDX_GITHUBPOOL_RELEASES_NUM:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toUInt());
            break;
        case IDX_GITHUBPOOL_CONTRIBUTORS_NUM:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toUInt());
            break;
        case IDX_GITHUBPOOL_ISSUSES_OPEN_NUM:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toUInt());
            break;
        case IDX_GITHUBPOOL_ISSUSES_CLOSED_NUM:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toUInt());
            break;
        case IDX_GITHUBPOOL_PULLREQ_OPEN_NUM:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toUInt());
            break;
        case IDX_GITHUBPOOL_PULLREQ_CLOSED_NUM:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toUInt());
            break;
        case IDX_GITHUBPOOL_STARS_NUM:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toUInt());
            break;
        case IDX_GITHUBPOOL_FORKS_NUM:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toUInt());
            break;
        default:
            break;
        }
    }
    if(!query.exec()) {
        qDebug() << "error insert into " << T_GITHUBPOOL;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::insertIntoMarketsTable(const QHash<int, QVariant> &roles)
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
        case IDX_MARKETSPOOL_CURRENCY_ID:
            sql_cells.append(CELL_MARKETSPOOL_CURRENCY_ID);
            break;
        case IDX_MARKETSPOOL_MARKET:
            sql_cells.append(CELL_MARKETSPOOL_MARKET);
            break;
        case IDX_MARKETSPOOL_EX_SYMBOL:
            sql_cells.append(CELL_MARKETSPOOL_EX_SYMBOL);
            break;
        case IDX_MARKETSPOOL_VOL24_USD:
            sql_cells.append(CELL_MARKETSPOOL_VOL24_USD);
            break;
        case IDX_MARKETSPOOL_VOL24_BTC:
            sql_cells.append(CELL_MARKETSPOOL_VOL24_BTC);
            break;
        case IDX_MARKETSPOOL_VOL_PERC:
            sql_cells.append(CELL_MARKETSPOOL_VOL_PERC);
            break;
        case IDX_MARKETSPOOL_PRICE_USD:
            sql_cells.append(CELL_MARKETSPOOL_PRICE_USD);
            break;
        case IDX_MARKETSPOOL_PRICE_BTC:
            sql_cells.append(CELL_MARKETSPOOL_PRICE_BTC);
            break;
        case IDX_MARKETSPOOL_URL:
            sql_cells.append(CELL_MARKETSPOOL_URL);
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
    sql_cmd = QString("INSERT OR REPLACE INTO " T_MARKETSPOOL " ( ");
    sql_cmd += sql_cells.join(", ");
    sql_cmd += ") VALUES ( ";
    sql_cmd += sql_values.join(", ");
    sql_cmd += " );";

    query.prepare(sql_cmd);

    for(int i = 0; i < keys.size(); i++) {
        int key = keys.at(i);
        switch (key) {
        case IDX_MARKETSPOOL_CURRENCY_ID:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_MARKETSPOOL_MARKET:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_MARKETSPOOL_EX_SYMBOL:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_MARKETSPOOL_VOL24_USD:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toDouble());
            break;
        case IDX_MARKETSPOOL_VOL24_BTC:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toDouble());
            break;
        case IDX_MARKETSPOOL_VOL_PERC:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toDouble());
            break;
        case IDX_MARKETSPOOL_PRICE_USD:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toDouble());
            break;
        case IDX_MARKETSPOOL_PRICE_BTC:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toDouble());
            break;
        case IDX_MARKETSPOOL_URL:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        default:
            break;
        }
    }
    if(!query.exec()) {
        qDebug() << "error insert into " << T_MARKETSPOOL;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::clearTableByIdx(int table_idx, QString condition)
{
    QSqlQuery query;
    QString sql_cmd = QString("DELETE FROM %1%2;")
            .arg(getTableNameByIdx(table_idx))
            .arg((condition.length() > 0) ? QString(" WHERE %1").arg(condition) : "");

    query.prepare(sql_cmd);
    if(!query.exec()) {
        qDebug() << "error clear " << getTableNameByIdx(table_idx);
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::copyTable(int source_table_idx, int destination_table_idx, bool orReplace, QString condition)
{
    QSqlQuery query;
    QString sql_cmd = QString("INSERT%1 INTO %2 SELECT * FROM %3%4;")
            .arg((orReplace?" OR REPLACE":""))
            .arg(getTableNameByIdx(destination_table_idx))
            .arg(getTableNameByIdx(source_table_idx))
            .arg((condition.length() > 0) ? QString(" WHERE %1").arg(condition) : "");

    query.prepare(sql_cmd);
    if(!query.exec()) {
        qDebug() << "error copy into " << getTableNameByIdx(destination_table_idx) << " from " << getTableNameByIdx(source_table_idx);
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::copyCurrenciesBetweenTablesByNotExist(int source_select_items_table_idx, int source_comparable_table_idx, int destination_finded_item_table_idx, bool orReplace)
{
    QSqlQuery query;
    QString sql_cmd = QString("INSERT%1 INTO %2 (SELECT FROM %3 WHERE ids NOT IN(SELECT ids FROM %4));")
            .arg((orReplace?" OR REPLACE":""))
            .arg(getTableNameByIdx(destination_finded_item_table_idx))
            .arg(getTableNameByIdx(source_select_items_table_idx))
            .arg(getTableNameByIdx(source_comparable_table_idx));

    query.prepare(sql_cmd);
    if(!query.exec()) {
        qDebug() << "error copy into " << getTableNameByIdx(destination_finded_item_table_idx) << " from " << getTableNameByIdx(source_select_items_table_idx);
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

QList<QHash<int, QVariant> > DataBase::selectFromCurrenciesTable(const QList<int> columns_ids)
{
    QList<QHash<int, QVariant> > result;
    QStringList sql_cells;

    if(columns_ids.empty())
        return result;    

    for(int i = 0; i < columns_ids.size(); i++) {
        switch (columns_ids.at(i)) {
        case IDX_CURRENCIES_ID:
            sql_cells.append(CELL_CURRENCIES_ID);
            break;
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
        case IDX_CURRENCIES_CMC_PAGE_URL:
            sql_cells.append(CELL_CURRENCIES_CMC_PAGE_URL);
            break;
        default:
            break;
        }
    }

    QString sql_cmd = QString("SELECT %1 FROM %2;").arg(sql_cells.join(", ")).arg(T_CURRENCIES);
    QSqlQuery qry;
    qry.prepare(sql_cmd);

    if(!qry.exec()) {
        qDebug() << "prepare_query:" << sql_cmd;
        qDebug() << "last_query:" << qry.lastQuery();
        qDebug() << "Error select, " << qry.lastError().text();
        return result;
    }
    int counter = 0;
    while(qry.next()) {
        QHash<int,QVariant> item;
        for(int i=0; i<columns_ids.size(); i++) {
            item.insert(columns_ids.at(i), qry.value(DataBase::getCellNameByIdx(IDX_TABLE_CURRENCIES, columns_ids.at(i))));
        }
//        qDebug() << item.value(1);
        result.append(item);
        ++counter;
    }
//    qDebug()<<"counter="<<counter;
    return result;
}

QString DataBase::getDatabaseFilename() {
    return base_path.path() + QDir::separator() + DATABASE_NAME;
}

QString DataBase::getCellNameByIdx(int table_idx, int cell_idx)
{
    switch (table_idx) {
    case IDX_TABLE_CURRENCIES:
        switch (cell_idx) {
        case IDX_CURRENCIES_ID:                     return CELL_CURRENCIES_ID;
        case IDX_CURRENCIES_NAME:                   return CELL_CURRENCIES_NAME;
        case IDX_CURRENCIES_SYMBOL:                 return CELL_CURRENCIES_SYMBOL;
        case IDX_CURRENCIES_RANK:                   return CELL_CURRENCIES_RANK;
        case IDX_CURRENCIES_PRICE_USD:              return CELL_CURRENCIES_PRICE_USD;
        case IDX_CURRENCIES_PRICE_BTC:              return CELL_CURRENCIES_PRICE_BTC;
        case IDX_CURRENCIES_VOL24H_USD:             return CELL_CURRENCIES_VOL24H_USD;
        case IDX_CURRENCIES_MARKETCAP_USD:          return CELL_CURRENCIES_MARKETCAP_USD;
        case IDX_CURRENCIES_AVAIBLE_SUPPLY:         return CELL_CURRENCIES_AVAIBLE_SUPPLY;
        case IDX_CURRENCIES_TOTAL_SUPPLY:           return CELL_CURRENCIES_TOTAL_SUPPLY;
        case IDX_CURRENCIES_MAX_SUPPLY:             return CELL_CURRENCIES_MAX_SUPPLY;
        case IDX_CURRENCIES_PERCENT_CH_1H:          return CELL_CURRENCIES_PERCENT_CH_1H;
        case IDX_CURRENCIES_PERCENT_CH_24H:         return CELL_CURRENCIES_PERCENT_CH_24H;
        case IDX_CURRENCIES_PERCENT_CH_7D:          return CELL_CURRENCIES_PERCENT_CH_7D;
        case IDX_CURRENCIES_LAST_UPDATE_DATE:       return CELL_CURRENCIES_LAST_UPDATE_DATE;
        case IDX_CURRENCIES_CMC_PAGE_URL:           return CELL_CURRENCIES_CMC_PAGE_URL;
        case IDX_CURRENCIES_SL_TYPE:                return CELL_CURRENCIES_SL_TYPE;
        case IDX_CURRENCIES_SL_MINEABLE:            return CELL_CURRENCIES_SL_MINEABLE;
        case IDX_CURRENCIES_SL_SITE_URLS:           return CELL_CURRENCIES_SL_SITE_URLS;
        case IDX_CURRENCIES_SL_ANNONCEMENT_URLS:    return CELL_CURRENCIES_SL_ANNONCEMENT_URLS;
        case IDX_CURRENCIES_SL_CHAT_URLS:           return CELL_CURRENCIES_SL_CHAT_URLS;
        case IDX_CURRENCIES_SL_EXPLORER_URLS:       return CELL_CURRENCIES_SL_EXPLORER_URLS;
        case IDX_CURRENCIES_SL_MSGBOARD_URLS:       return CELL_CURRENCIES_SL_MSGBOARD_URLS;
        case IDX_CURRENCIES_SL_SRC_URLS:            return CELL_CURRENCIES_SL_SRC_URLS;
        default:
            break;
        }
        break;
    case IDX_TABLE_CURRENCIES_MEM:
        switch (cell_idx) {
        case IDX_CURRENCIES_MEM_ID:                 return CELL_CURRENCIES_MEM_ID;
        case IDX_CURRENCIES_MEM_MANUAL_UPD_DATE:    return CELL_CURRENCIES_MEM_MANUAL_UPD_DATE;
        case IDX_CURRENCIES_MEM_APP_AREAS_KEY:      return CELL_CURRENCIES_MEM_APP_AREAS_KEY;
        case IDX_CURRENCIES_MEM_REV_IDEA:           return CELL_CURRENCIES_MEM_REV_IDEA;
        case IDX_CURRENCIES_MEM_BASE_PLATFORM_KEY:  return CELL_CURRENCIES_MEM_BASE_PLATFORM_KEY;
        case IDX_CURRENCIES_MEM_CON_ALG_KEY:        return CELL_CURRENCIES_MEM_CON_ALG_KEY;
        case IDX_CURRENCIES_MEM_WP_URL:             return CELL_CURRENCIES_MEM_WP_URL;
        case IDX_CURRENCIES_MEM_RM_URL:             return CELL_CURRENCIES_MEM_RM_URL;
        case IDX_CURRENCIES_MEM_F_INTER_COMP:       return CELL_CURRENCIES_MEM_F_INTER_COMP;
        case IDX_CURRENCIES_MEM_F_WP_IN_CV:         return CELL_CURRENCIES_MEM_F_WP_IN_CV;
        case IDX_CURRENCIES_MEM_CONTACTS:           return CELL_CURRENCIES_MEM_CONTACTS;
        default:
            break;
        }
        break;
    case IDX_TABLE_CONSENSUSALG:
        switch (cell_idx) {
        case IDX_CONSENSUSALG_NAME:                 return CELL_CONSENSUSALG_NAME;
        case IDX_CONSENSUSALG_INFO:                 return CELL_CONSENSUSALG_INFO;
        default:
            break;
        }
        break;
    case IDX_TABLE_AREAS:
        switch (cell_idx) {
        case IDX_AREAS_NAME:                        return CELL_AREAS_NAME;
        case IDX_AREAS_COMMENT:                     return CELL_AREAS_COMMENT;
        default:
            break;
        }
        break;
    case IDX_TABLE_DEVINFO:
        switch (cell_idx) {
        case IDX_DEVINFO_CURRENCY_ID:               return CELL_DEVINFO_CURRENCY_ID;
        case IDX_DEVINFO_PROJECT_NAME:              return CELL_DEVINFO_PROJECT_NAME;
        case IDX_DEVINFO_START_DATETIME_STR:        return CELL_DEVINFO_START_DATETIME_STR;
        case IDX_DEVINFO_LANGUAGE_USE:              return CELL_DEVINFO_LANGUAGE_USE;
        case IDX_DEVINFO_LICENSE:                   return CELL_DEVINFO_LICENSE;
        case IDX_DEVINFO_F_IS_FORK:                 return CELL_DEVINFO_F_IS_FORK;
        default:
            break;
        }
        break;
    case IDX_TABLE_GITHUBPOOL:
        switch (cell_idx) {
        case IDX_GITHUBPOOL_CURRENCY_ID:            return CELL_GITHUBPOOL_CURRENCY_ID;
        case IDX_GITHUBPOOL_PROJECT_NAME:           return CELL_GITHUBPOOL_PROJECT_NAME;
        case IDX_GITHUBPOOL_COMMITS_NUM:            return CELL_GITHUBPOOL_COMMITS_NUM;
        case IDX_GITHUBPOOL_BRANCHES_NUM:           return CELL_GITHUBPOOL_BRANCHES_NUM;
        case IDX_GITHUBPOOL_RELEASES_NUM:           return CELL_GITHUBPOOL_RELEASES_NUM;
        case IDX_GITHUBPOOL_CONTRIBUTORS_NUM:       return CELL_GITHUBPOOL_CONTRIBUTORS_NUM;
        case IDX_GITHUBPOOL_ISSUSES_OPEN_NUM:       return CELL_GITHUBPOOL_ISSUSES_OPEN_NUM;
        case IDX_GITHUBPOOL_ISSUSES_CLOSED_NUM:     return CELL_GITHUBPOOL_ISSUSES_CLOSED_NUM;
        case IDX_GITHUBPOOL_PULLREQ_OPEN_NUM:       return CELL_GITHUBPOOL_PULLREQ_OPEN_NUM;
        case IDX_GITHUBPOOL_PULLREQ_CLOSED_NUM:     return CELL_GITHUBPOOL_PULLREQ_CLOSED_NUM;
        case IDX_GITHUBPOOL_STARS_NUM:              return CELL_GITHUBPOOL_STARS_NUM;
        case IDX_GITHUBPOOL_FORKS_NUM:              return CELL_GITHUBPOOL_FORKS_NUM;
        default:
            break;
        }
        break;
    case IDX_TABLE_MARKETSPOOL:
        switch (cell_idx) {
        case IDX_MARKETSPOOL_CURRENCY_ID:           return CELL_MARKETSPOOL_CURRENCY_ID;
        case IDX_MARKETSPOOL_MARKET:                return CELL_MARKETSPOOL_MARKET;
        case IDX_MARKETSPOOL_EX_SYMBOL:             return CELL_MARKETSPOOL_EX_SYMBOL;
        case IDX_MARKETSPOOL_VOL24_USD:             return CELL_MARKETSPOOL_VOL24_USD;
        case IDX_MARKETSPOOL_VOL24_BTC:             return CELL_MARKETSPOOL_VOL24_BTC;
        case IDX_MARKETSPOOL_VOL_PERC:              return CELL_MARKETSPOOL_VOL_PERC;
        case IDX_MARKETSPOOL_PRICE_USD:             return CELL_MARKETSPOOL_PRICE_USD;
        case IDX_MARKETSPOOL_PRICE_BTC:             return CELL_MARKETSPOOL_PRICE_BTC;
        case IDX_MARKETSPOOL_URL:                   return CELL_MARKETSPOOL_URL;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return "";
}

QString DataBase::getTableNameByIdx(int table_idx)
{
    switch (table_idx) {
    case IDX_TABLE_CURRENCIES:                      return T_CURRENCIES;
    case IDX_TABLE_CURRENCIES_PREV:                 return T_CURRENCIES_PREV;
    case IDX_TABLE_CURRENCIES_DEAD:                 return T_CURRENCIES_DEAD;
    case IDX_TABLE_CURRENCIES_BORN:                 return T_CURRENCIES_BORN;
    case IDX_TABLE_CURRENCIES_MEM:                  return T_CURRENCIES_MEM;
    case IDX_TABLE_CONSENSUSALG:                    return T_CONSENSUSALG;
    case IDX_TABLE_AREAS:                           return T_AREAS;
    case IDX_TABLE_DEVINFO:                         return T_DEVINFO;
    case IDX_TABLE_GITHUBPOOL:                      return T_GITHUBPOOL;
    case IDX_TABLE_MARKETSPOOL:                     return T_MARKETSPOOL;
    default:
        break;
    }
    return "";
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

    db.setDatabaseName(getDatabaseFilename());

    //qDebug() << base_path.path() + QDir::separator() + DATABASE_NAME;
    if(db.open()) {
        db.exec("PRAGMA synchronous = OFF");
        db.exec("PRAGMA journal_mode = MEMORY");
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
bool DataBase::createCurrenciesTable(int table_idx, bool flagIfNotExist)
{
    QSqlQuery query;
    QString sql = QString("CREATE TABLE %1%2 ("
                                                         "'" CELL_CURRENCIES_ID "'" " TEXT UNIQUE PRIMARY KEY, "
                                                         "'" CELL_CURRENCIES_NAME "'" " VARCHAR(255) NOT NULL, "
                                                         "'" CELL_CURRENCIES_SYMBOL "'" " VARCHAR(10) NOT NULL, "
                                                         "'" CELL_CURRENCIES_RANK "'" " INTEGER, "
                                                         "'" CELL_CURRENCIES_PRICE_USD "'" " REAL NOT NULL, "
                                                         "'" CELL_CURRENCIES_PRICE_BTC "'" " REAL NOT NULL, "
                                                         "'" CELL_CURRENCIES_VOL24H_USD "'" " REAL NOT NULL, "
                                                         "'" CELL_CURRENCIES_MARKETCAP_USD "'" " REAL NOT NULL, "
                                                         "'" CELL_CURRENCIES_AVAIBLE_SUPPLY "'" " UNSIGNED BIG INT NOT NULL, "
                                                         "'" CELL_CURRENCIES_TOTAL_SUPPLY "'" " UNSIGNED BIG INT NOT NULL, "
                                                         "'" CELL_CURRENCIES_MAX_SUPPLY "'" " UNSIGNED BIG INT NOT NULL, "
                                                         "'" CELL_CURRENCIES_PERCENT_CH_1H "'" " REAL NOT NULL, "
                                                         "'" CELL_CURRENCIES_PERCENT_CH_24H "'" " REAL NOT NULL, "
                                                         "'" CELL_CURRENCIES_PERCENT_CH_7D "'" " REAL NOT NULL, "
                                                         "'" CELL_CURRENCIES_LAST_UPDATE_DATE "'" " UNSIGNED BIG INT NOT NULL, "
                                                         "'" CELL_CURRENCIES_CMC_PAGE_URL "'" " TEXT , "
                                                         "'" CELL_CURRENCIES_SL_TYPE "'" " INTEGER, "
                                                         "'" CELL_CURRENCIES_SL_MINEABLE "'" " BOOLEAN, "
                                                         "'" CELL_CURRENCIES_SL_SITE_URLS "'" " TEXT, "
                                                         "'" CELL_CURRENCIES_SL_ANNONCEMENT_URLS "'" " TEXT, "
                                                         "'" CELL_CURRENCIES_SL_CHAT_URLS "'" " TEXT, "
                                                         "'" CELL_CURRENCIES_SL_EXPLORER_URLS "'" " TEXT, "
                                                         "'" CELL_CURRENCIES_SL_MSGBOARD_URLS "'" " TEXT, "
                                                         "'" CELL_CURRENCIES_SL_SRC_URLS "'" " TEXT"
                                                     " );")
            .arg((flagIfNotExist?"IF NOT EXISTS ":""))
            .arg(getTableNameByIdx(table_idx));

    if(!query.exec(sql)) {
        qDebug() << "DataBase: error of create " << getTableNameByIdx(table_idx);
        qDebug() << query.lastError().text();
        qDebug() << query.executedQuery();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::createCurrenciesMemTable(bool flagIfNotExist)
{
    QSqlQuery query;
    QString sql = QString("CREATE TABLE %1" T_CURRENCIES_MEM " ("
                                                             "'" CELL_CURRENCIES_MEM_ID "'" " TEXT UNIQUE PRIMARY KEY,"
                                                             "'" CELL_CURRENCIES_MEM_MANUAL_UPD_DATE "'" " UNSIGNED BIG INT NOT NULL, "
                                                             "'" CELL_CURRENCIES_MEM_APP_AREAS_KEY "'" " TEXT, "
                                                             "'" CELL_CURRENCIES_MEM_REV_IDEA "'" " TEXT, "
                                                             "'" CELL_CURRENCIES_MEM_BASE_PLATFORM_KEY "'" " TEXT, "
                                                             "'" CELL_CURRENCIES_MEM_CON_ALG_KEY "'" " TEXT, "
                                                             "'" CELL_CURRENCIES_MEM_WP_URL "'" " TEXT, "
                                                             "'" CELL_CURRENCIES_MEM_RM_URL "'" " TEXT, "
                                                             "'" CELL_CURRENCIES_MEM_F_INTER_COMP "'" " BOOLEAN, "
                                                             "'" CELL_CURRENCIES_MEM_F_WP_IN_CV "'" " BOOLEAN, "
                                                             "'" CELL_CURRENCIES_MEM_CONTACTS "'" " TEXT"
                                                         " );").arg((flagIfNotExist?"IF NOT EXISTS ":""));
    if(!query.exec(sql)) {
        qDebug() << "DataBase: error of create " << T_CURRENCIES_MEM;
        qDebug() << query.lastError().text();
        qDebug() << query.executedQuery();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::createConsensusAlgTable(bool flagIfNotExist)
{
    QSqlQuery query;
    QString sql = QString("CREATE TABLE %1" T_CONSENSUSALG " ("
                                                           "'id' INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                           "'" CELL_CONSENSUSALG_NAME "'" " VARCHAR(20) NOT NULL, "
                                                           "'" CELL_CONSENSUSALG_INFO "'" " TEXT"
                                                       " );").arg((flagIfNotExist?"IF NOT EXISTS ":""));
    if(!query.exec(sql)) {
        qDebug() << "DataBase: error of create " << T_CONSENSUSALG;
        qDebug() << query.lastError().text();
        qDebug() << query.executedQuery();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::createAppAreasTable(bool flagIfNotExist)
{
    QSqlQuery query;
    QString sql = QString("CREATE TABLE %1" T_AREAS " ("
                                                    "'id' INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                    "'" CELL_AREAS_NAME "'" " VARCHAR(50) NOT NULL, "
                                                    "'" CELL_AREAS_COMMENT "'" " TEXT"
                                                " );").arg((flagIfNotExist?"IF NOT EXISTS ":""));

    if(!query.exec(sql)) {
        qDebug() << "DataBase: error of create " << T_AREAS;
        qDebug() << query.lastError().text();
        qDebug() << query.executedQuery();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::createDevInfoTable(bool flagIfNotExist)
{
    QSqlQuery query;
    QString sql = QString("CREATE TABLE %1" T_DEVINFO " ("
                                                      "'id' INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                      "'" CELL_DEVINFO_CURRENCY_ID "'" " TEXT NOT NULL, "
                                                      "'" CELL_DEVINFO_PROJECT_NAME "'" " VARCHAR(50) NOT NULL, "
                                                      "'" CELL_DEVINFO_START_DATETIME_STR "'" "VARCHAR(30) NOT NULL, "
                                                      "'" CELL_DEVINFO_LANGUAGE_USE "'" " VARCHAR(20) NOT NULL, "
                                                      "'" CELL_DEVINFO_LICENSE "'" " VARCHAR(50) NOT NULL, "
                                                      "'" CELL_DEVINFO_F_IS_FORK "'" " BOOLEAN"
                                                  " );").arg((flagIfNotExist?"IF NOT EXISTS ":""));
    if(!query.exec(sql)) {
        qDebug() << "DataBase: error of create " << T_DEVINFO;
        qDebug() << query.lastError().text();
        qDebug() << query.executedQuery();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::createGithubHistoryPoolTable(bool flagIfNotExist)
{
    QSqlQuery query;
    QString sql = QString("CREATE TABLE %1" T_GITHUBPOOL " ("
                                                         "'id' INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                         "'" CELL_GITHUBPOOL_CURRENCY_ID "'" " TEXT NOT NULL, "
                                                         "'" CELL_GITHUBPOOL_PROJECT_NAME "'" " VARCHAR(50) NOT NULL, "
                                                         "'" CELL_GITHUBPOOL_COMMITS_NUM "'" " INTEGER NOT NULL, "
                                                         "'" CELL_GITHUBPOOL_BRANCHES_NUM "'" " INTEGER NOT NULL, "
                                                         "'" CELL_GITHUBPOOL_RELEASES_NUM "'" " INTEGER NOT NULL, "
                                                         "'" CELL_GITHUBPOOL_CONTRIBUTORS_NUM "'" " INTEGER NOT NULL, "
                                                         "'" CELL_GITHUBPOOL_ISSUSES_OPEN_NUM "'" " INTEGER NOT NULL, "
                                                         "'" CELL_GITHUBPOOL_ISSUSES_CLOSED_NUM "'" " INTEGER NOT NULL, "
                                                         "'" CELL_GITHUBPOOL_PULLREQ_OPEN_NUM "'" " INTEGER NOT NULL, "
                                                         "'" CELL_GITHUBPOOL_PULLREQ_CLOSED_NUM "'" " INTEGER NOT NULL, "
                                                         "'" CELL_GITHUBPOOL_STARS_NUM "'" " INTEGER NOT NULL, "
                                                         "'" CELL_GITHUBPOOL_FORKS_NUM "'" " INTEGER NOT NULL"
                                                     " );").arg((flagIfNotExist?"IF NOT EXISTS ":""));

    if(!query.exec(sql)) {
        qDebug() << "DataBase: error of create " << T_GITHUBPOOL;
        qDebug() << query.lastError().text();
        qDebug() << query.executedQuery();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::createMarketsTable(bool flagIfNotExist)
{
    QSqlQuery query;
    QString sql = QString("CREATE TABLE %1" T_MARKETSPOOL " ("
                                                          "'id' INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                          "'" CELL_MARKETSPOOL_CURRENCY_ID "'" " TEXT NOT NULL, "
                                                          "'" CELL_MARKETSPOOL_MARKET "'" " VARCHAR(20) NOT NULL, "
                                                          "'" CELL_MARKETSPOOL_EX_SYMBOL "'" " VARCHAR(20) NOT NULL, "
                                                          "'" CELL_MARKETSPOOL_VOL24_USD "'" " REAL NOT NULL, "
                                                          "'" CELL_MARKETSPOOL_VOL24_BTC "'" " REAL NOT NULL, "
                                                          "'" CELL_MARKETSPOOL_VOL_PERC "'" " REAL NOT NULL, "
                                                          "'" CELL_MARKETSPOOL_PRICE_USD "'" " REAL NOT NULL, "
                                                          "'" CELL_MARKETSPOOL_PRICE_BTC "'" " REAL NOT NULL, "
                                                          "'" CELL_MARKETSPOOL_URL "'" " TEXT"
                                                      " );").arg((flagIfNotExist?"IF NOT EXISTS ":""));
    if(!query.exec(sql)) {
        qDebug() << "DataBase: error of create " << T_MARKETSPOOL;
        qDebug() << query.lastError().text();
        qDebug() << query.executedQuery();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::createAllTables()
{
    if((!this->createCurrenciesTable(IDX_TABLE_CURRENCIES))
            || (!this->createCurrenciesTable(IDX_TABLE_CURRENCIES_PREV))
            || (!this->createCurrenciesTable(IDX_TABLE_CURRENCIES_DEAD))
            || (!this->createCurrenciesTable(IDX_TABLE_CURRENCIES_BORN))
            || (!this->createCurrenciesMemTable())
            || (!this->createConsensusAlgTable())
            || (!this->createAppAreasTable())
            || (!this->createDevInfoTable())
            || (!this->createGithubHistoryPoolTable())
            || (!this->createMarketsTable())
            ) {
        return false;
    }
    return true;
}
