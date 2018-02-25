#ifndef TASKRESULT_H
#define TASKRESULT_H

#include <QString>
#include <QVariant>
#include <QHash>

class TaskResult {
public:
    TaskResult()
    {
        error = 0;
        flag_show_output_msg = false;
    }
    int error;

    QVariant data;
    QHash<QString,QVariant> attributes;

    // для сообщений
    bool flag_show_output_msg;
    QString text_output_msg;
    QString text_output_title;
};

#endif // TASKRESULT_H
