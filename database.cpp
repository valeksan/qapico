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
bool DataBase::insertIntoCurrenciesTable(const QHash<int,QVariant> &roles)
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
        case IDX_DEVINFO_CURRENCY_SYMBOL:
            sql_cells.append(CELL_DEVINFO_CURRENCY_SYMBOL);
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
        case IDX_DEVINFO_CURRENCY_SYMBOL:
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
        case IDX_GITHUBPOOL_CURRENCY_SYMBOL:
            sql_cells.append(CELL_GITHUBPOOL_CURRENCY_SYMBOL);
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
        case IDX_GITHUBPOOL_CURRENCY_SYMBOL:
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
        case IDX_MARKETSPOOL_CURRENCY_SYMBOL:
            sql_cells.append(CELL_MARKETSPOOL_CURRENCY_SYMBOL);
            break;
        case IDX_MARKETSPOOL_MARKET:
            sql_cells.append(CELL_MARKETSPOOL_MARKET);
            break;
        case IDX_MARKETSPOOL_PAIR:
            sql_cells.append(CELL_MARKETSPOOL_PAIR);
            break;
        case IDX_MARKETSPOOL_VOL_24:
            sql_cells.append(CELL_MARKETSPOOL_VOL_24);
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
        case IDX_MARKETSPOOL_CURRENCY_SYMBOL:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_MARKETSPOOL_MARKET:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_MARKETSPOOL_PAIR:
            query.bindValue(QString(":val_%1").arg(key), roles.value(key).toString());
            break;
        case IDX_MARKETSPOOL_VOL_24:
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
bool DataBase::createCurrenciesTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " T_CURRENCIES " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            CELL_CURRENCIES_NAME " VARCHAR(255) NOT NULL,"
                            CELL_CURRENCIES_SYMBOL " VARCHAR(10) NOT NULL,"
                            CELL_CURRENCIES_RANK " INTEGER,"
                            CELL_CURRENCIES_PRICE_USD " REAL NOT NULL,"
                            CELL_CURRENCIES_PRICE_BTC " REAL NOT NULL,"
                            CELL_CURRENCIES_VOL24H_USD " REAL NOT NULL,"
                            CELL_CURRENCIES_MARKETCAP_USD " REAL NOT NULL,"
                            CELL_CURRENCIES_AVAIBLE_SUPPLY " UNSIGNED BIG INT NOT NULL,"
                            CELL_CURRENCIES_TOTAL_SUPPLY " UNSIGNED BIG INT NOT NULL,"
                            CELL_CURRENCIES_MAX_SUPPLY " UNSIGNED BIG INT NOT NULL,"
                            CELL_CURRENCIES_PERCENT_CH_1H " REAL NOT NULL,"
                            CELL_CURRENCIES_PERCENT_CH_24H " REAL NOT NULL,"
                            CELL_CURRENCIES_PERCENT_CH_7D " REAL NOT NULL,"
                            CELL_CURRENCIES_LAST_UPDATE_DATE " UNSIGNED BIG INT NOT NULL,"
                            CELL_CURRENCIES_SL_TYPE " INTEGER,"
                            CELL_CURRENCIES_SL_MINEABLE " BOOLEAN,"
                            CELL_CURRENCIES_SL_SITE_URLS " TEXT,"
                            CELL_CURRENCIES_SL_ANNONCEMENT_URLS " TEXT,"
                            CELL_CURRENCIES_SL_CHAT_URLS " TEXT,"
                            CELL_CURRENCIES_SL_EXPLORER_URLS " TEXT,"
                            CELL_CURRENCIES_SL_MSGBOARD_URLS " TEXT,"
                            CELL_CURRENCIES_SL_SRC_URLS " TEXT"
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

bool DataBase::createCurrenciesMemTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " T_CURRENCIES_MEM " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            CELL_CURRENCIES_MEM_SYMBOL " VARCHAR(10) NOT NULL,"
                            CELL_CURRENCIES_MEM_MANUAL_UPD_DATE " UNSIGNED BIG INT NOT NULL,"
                            CELL_CURRENCIES_MEM_APP_AREAS_KEY " TEXT,"
                            CELL_CURRENCIES_MEM_REV_IDEA " TEXT,"
                            CELL_CURRENCIES_MEM_BASE_PLATFORM_KEY " TEXT,"
                            CELL_CURRENCIES_MEM_CON_ALG_KEY " TEXT,"
                            CELL_CURRENCIES_MEM_WP_URL " TEXT,"
                            CELL_CURRENCIES_MEM_RM_URL " TEXT,"
                            CELL_CURRENCIES_MEM_F_INTER_COMP " BOOLEAN,"
                            CELL_CURRENCIES_MEM_F_WP_IN_CV " BOOLEAN,"
                            CELL_CURRENCIES_MEM_CONTACTS " TEXT"
                        " )"
                    )) {
        qDebug() << "DataBase: error of create " << T_CURRENCIES_MEM;
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
                            CELL_AREAS_NAME " VARCHAR(50) NOT NULL,"
                            CELL_AREAS_COMMENT " TEXT"
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
                            CELL_DEVINFO_CURRENCY_SYMBOL " VARCHAR(10) NOT NULL,"
                            CELL_DEVINFO_PROJECT_NAME " VARCHAR(50) NOT NULL,"
                            CELL_DEVINFO_START_DATETIME_STR "VARCHAR(30) NOT NULL,"
                            CELL_DEVINFO_LANGUAGE_USE " VARCHAR(20) NOT NULL,"
                            CELL_DEVINFO_LICENSE " VARCHAR(50) NOT NULL,"
                            CELL_DEVINFO_F_IS_FORK " BOOLEAN"
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

bool DataBase::createGithubHistoryPoolTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " T_GITHUBPOOL " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            CELL_GITHUBPOOL_CURRENCY_SYMBOL " VARCHAR(10) NOT NULL,"
                            CELL_GITHUBPOOL_PROJECT_NAME " VARCHAR(50) NOT NULL,"
                            CELL_GITHUBPOOL_COMMITS_NUM " INTEGER NOT NULL,"
                            CELL_GITHUBPOOL_BRANCHES_NUM " INTEGER NOT NULL,"
                            CELL_GITHUBPOOL_RELEASES_NUM " INTEGER NOT NULL,"
                            CELL_GITHUBPOOL_CONTRIBUTORS_NUM " INTEGER NOT NULL,"
                            CELL_GITHUBPOOL_ISSUSES_OPEN_NUM " INTEGER NOT NULL,"
                            CELL_GITHUBPOOL_ISSUSES_CLOSED_NUM " INTEGER NOT NULL,"
                            CELL_GITHUBPOOL_PULLREQ_OPEN_NUM " INTEGER NOT NULL,"
                            CELL_GITHUBPOOL_PULLREQ_CLOSED_NUM " INTEGER NOT NULL,"
                            CELL_GITHUBPOOL_STARS_NUM " INTEGER NOT NULL,"
                            CELL_GITHUBPOOL_FORKS_NUM " INTEGER NOT NULL"
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
                            CELL_MARKETSPOOL_CURRENCY_SYMBOL " INTEGER NOT NULL,"
                            CELL_MARKETSPOOL_MARKET " VARCHAR(20) NOT NULL,"
                            CELL_MARKETSPOOL_PAIR " VARCHAR(20) NOT NULL,"
                            CELL_MARKETSPOOL_VOL_24 " REAL NOT NULL,"
                            CELL_MARKETSPOOL_VOL_PERC " REAL NOT NULL,"
                            CELL_MARKETSPOOL_PRICE_USD " REAL NOT NULL,"
                            CELL_MARKETSPOOL_PRICE_BTC " REAL NOT NULL"
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

bool DataBase::createAllTables()
{
    if((!this->createCurrenciesTable())
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
