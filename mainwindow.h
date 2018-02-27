#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>

#include <QDebug>

#include "database.h"

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
    DataBase *db;

};

Q_DECLARE_METATYPE(TaskResult)

#endif // MAINWINDOW_H
