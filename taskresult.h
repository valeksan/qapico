#ifndef TASKRESULT_H
#define TASKRESULT_H

#include <QObject>
#include <QString>

#include "enums.h"

class TaskResult {
public:
    TaskResult()
    {
        error = ERR_TASK_OK;
        errorExt1 = -1;
        errorExt2 = -1;
        toShowErrorOutputMsg = false;
    }
    int error;
    int errorExt1;
    int errorExt2;
    QString textOutputMsg;
    QString textOutputTitle;
    bool toShowErrorOutputMsg;
};
Q_DECLARE_METATYPE(TaskResult)

#endif // TASKRESULT_H
