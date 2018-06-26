#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QMessageBox>

#include <QDebug>

#include "database.h"
#include "exts_funcs.h"
#include "parser.h"
#include "downloader.h"
#include "core.h"

#include "enums.h"
#include "taskresult.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void registerTasks();

    void displayCurrenciesFromBase(int table_currencies_idx = IDX_TABLE_CURRENCIES);

private slots:
    void on_pushButtonUpdate_clicked();

    void slotFinishedTask(long id, int type, QVariantList argsList, QVariant result);

private:
    Ui::MainWindow *ui;
    DataBase *db;
    Core *core;

    bool m_isBaseCurrenciesInit;
    bool m_isInfoCurrenciesInit;
    bool m_isDevCurrenciesInit;

public:
signals:
    void showWarrningMsgDialog(QString title, QString msg, bool alert = false);

};

#endif // MAINWINDOW_H
