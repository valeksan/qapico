#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QAtomicPointer>

#include <QDebug>

#include "core.h"
#include "downloader.h"
#include "taskresult.h"
#include "database.h"

#include <qgumbodocument.h>
#include <qgumbonode.h>

namespace Ui {
class MainWindow;
}

enum Tasks
{
    TASK_UPDATE,
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonUpdate_clicked();

private:
    Ui::MainWindow *ui;
    Core *m_pCore;
    DataBase *db;

    QHash<QString,QUrl> m_currenciesPagesForParse;
    QMutex m_mutex;

};

Q_DECLARE_METATYPE(TaskResult)

#endif // MAINWINDOW_H
