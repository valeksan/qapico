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

#include "database_names.h"

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

    bool inserIntoCurrenciesTable(const QHash<int,QVariant> &roles);

    bool inserIntoCurrenciesTableManualEditContentInfo(const QVariantList &data);
    bool inserIntoCurrenciesTableManualEditContactInfo(const QVariantList &data);
    bool inserIntoRoadmapDatesPoolTable(const QVariantList &data);
    bool inserIntoTokenAlgTable(const QVariantList &data);
    bool inserIntoAppAreasTable(const QVariantList &data);
    bool inserIntoTechnologiesThreadsPoolTable(const QVariantList &data);
    bool inserIntoTechnologiesTable(const QVariantList &data);
    bool inserIntoGithubHistoryPoolTable(const QVariantList &data);
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
    bool createRoadmapDatesPoolTable();
    bool createTokenAlgTable();
    bool createAppAreasTable();
    bool createDevInfoTable();
    bool createTechnologiesThreadsPoolTable();
    bool createTechnologiesTable();
    bool createGithubHistoryPoolTable();
    bool createMarketsTable();

signals:

public slots:
};

#endif // DATABASE_H
