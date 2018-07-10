#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QMessageBox>
#include <QTableWidgetItem>

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

class TableNumberItem : public QTableWidgetItem
{
public:
    TableNumberItem(const QString txt = QString("0"))
        :QTableWidgetItem(txt)
    {
    }

    bool operator < (const QTableWidgetItem &other) const
    {
        QString str1 = text();
        QString str2 = other.text();

        if (str1[0] == '$') str1.remove(0, 1);
        else if(str1[1] == '$') str1.remove(1, 1);

        if (str2[0] == '$') str2.remove(0, 1);
        else if(str2[1] == '$') str2.remove(1, 1);

//        if(str1.length() > 0 && str2.length() > 0) {
//            if (str1[0] == '$' || str1[0] == '+') {
//                str1.remove(0, 1);
//                str2.remove(0, 1); // we assume both items have the same format
//                if(str1.length() > 0 && str2.length() > 0) {
//                    if (str1[0] == '$') str1.remove(0, 1);
//                    if (str1[1] == '$') str1.remove(1, 1);
//                }
//            }
//        }

        if (str1[str1.length() - 1] == '%') {
            str1.chop(1);
            str2.chop(1); // this works for "N%" and for "N %" formatted strings
        }

//        double f1 = str1.toDouble();
//        double f2 = str2.toDouble();

        return str1.toDouble() < str2.toDouble();
    }
};

class TableSimpleItem : public QTableWidgetItem
{
public:
    TableSimpleItem(const QString txt = QString(""))
        :QTableWidgetItem(txt)
    {
    }

    bool operator < (const QTableWidgetItem &other) const
    {
        QString str1 = text();
        QString str2 = other.text();

        return str1 < str2;
    }
};

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
    QString getStrCalcPercentIncreaseValue(double percent, int precision);

    void on_pushButtonActual_clicked();

    void on_pushButtonPrevScan_clicked();

    void on_pushButtonNewProjects_clicked();

    void on_pushButtonDeadProjects_clicked();

private:
    Ui::MainWindow *m_pUi;
    DataBase *m_pDb;
    Core *m_pCore;
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
