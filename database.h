#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDir>
#include <QDate>
#include <QDebug>
#include <QByteArray>
#include <QSqlDriver>

#include "database_names.h"

//#include <sqlite3.h>

//class QueryResult {
//public:
//    QueryResult() {}

//};

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
    /* Методы для непосредственной работы с классом
     * Подключение к базе данных и вставка записей в таблицу
     * */
    void connectToDataBase();

    bool insertIntoCurrenciesTable(const QHash<int,QVariant> &roles, bool orReplace = true);
    bool insertIntoCurrenciesMemTable(const QHash<int,QVariant> &roles);
    bool insertIntoConsensusAlgTable(const QHash<int,QVariant> &roles);
    bool insertIntoAreasTable(const QHash<int,QVariant> &roles);
    bool insertIntoDevInfoTable(const QHash<int,QVariant> &roles);
    bool insertIntoGithubHistoryPoolTable(const QHash<int,QVariant> &roles);
    bool insertIntoMarketsTable(const QHash<int,QVariant> &roles);

    QList<QHash<int,QVariant> > selectFromCurrenciesTable(const QList<int> columns_ids);

    QString getDatabaseFilename();

    static QString getCellNameByIdx(int table_idx, int cell_idx);

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

    bool createCurrenciesTable(bool flagIfNotExist = false);
    bool createCurrenciesMemTable(bool flagIfNotExist = false);
    bool createConsensusAlgTable(bool flagIfNotExist = false);
    bool createAppAreasTable(bool flagIfNotExist = false);
    bool createDevInfoTable(bool flagIfNotExist = false);
    bool createGithubHistoryPoolTable(bool flagIfNotExist = false);
    bool createMarketsTable(bool flagIfNotExist = false);

    bool createAllTables();

signals:

public slots:
};

#endif // DATABASE_H
