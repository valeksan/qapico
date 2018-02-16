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
            || (!this->createContactsTable())
            || (!this->createDevInfoTable())
            || (!this->createGithubHistoryPoolTable())
            || (!this->createInfoTable())
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
    //
    return true;
}

bool DataBase::createInfoTable()
{
    //
    return true;
}

bool DataBase::createRoadmapDatesPoolTable()
{
    //
    return true;
}

bool DataBase::createTokenAlgTable()
{
    //
    return true;
}

bool DataBase::createAppAreasTable()
{
    //
    return true;
}

bool DataBase::createDevInfoTable()
{
    //
    return true;
}

bool DataBase::createTechnologiesThreadsPoolTable()
{
    //
    return true;
}

bool DataBase::createTechnologiesTable()
{
    //
    return true;
}

bool DataBase::createGithubHistoryPoolTable()
{
    //
    return true;
}

bool DataBase::createContactsTable()
{
    //
    return true;
}

bool DataBase::createMarketsTable()
{
    //
    return true;
}
