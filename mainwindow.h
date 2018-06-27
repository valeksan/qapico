#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QMessageBox>
//#include <QStandardItemModel>

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
    void displayCurrenciesUpdate();
    void updateColorCheckTableButton();

private slots:
    void on_pushButtonUpdate_clicked();

    void slotFinishedTask(long id, int type, QVariantList argsList, QVariant result);

    QString getStrCalcDeltaIncreaseValue(double price, double percent, int precision, QString currency = "");

    void on_pushButtonActual_clicked();

    void on_pushButtonPrevScan_clicked();

    void on_pushButtonNewProjects_clicked();

    void on_pushButtonDeadProjects_clicked();

private:
    Ui::MainWindow *ui;
    DataBase *db;
    Core *core;
//    QStandardItemModel *model;
    Q_PROPERTY(int tableview_variant READ tableview_variant WRITE setTableview_variant NOTIFY tableview_variantChanged)

    bool m_isBaseCurrenciesInit;
    bool m_isInfoCurrenciesInit;
    bool m_isDevCurrenciesInit;

    int m_tableview_variant;

public:
int tableview_variant() const
{
    return m_tableview_variant;
}

public slots:
void setTableview_variant(int tableview_variant)
{
    if (m_tableview_variant == tableview_variant)
        return;

    m_tableview_variant = tableview_variant;
    emit tableview_variantChanged(m_tableview_variant);
}

signals:
    void showWarrningMsgDialog(QString title, QString msg, bool alert = false);

    void tableview_variantChanged(int tableview_variant);
};

#endif // MAINWINDOW_H
