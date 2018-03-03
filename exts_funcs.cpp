#include "exts_funcs.h"

bool isDownloaderPathExists()
{
    bool is_unix;
#ifdef Q_OS_UNIX
    is_unix = true;
#elif Q_OS_WINDOWS
    is_unix = false;
#endif
    QDir home_path = QDir::home();
    QDir apico_path = QDir(home_path.path()+QDir::separator()+(is_unix?".apico":"Apico"));
    QDir download_path = home_path.path()+QDir::separator()+apico_path.path()+QDir::separator()+(is_unix?"downloads":"Downloads");
    if(!apico_path.exists()) home_path.mkdir((is_unix?".apico":"Apico"));
    if(!download_path.exists() && apico_path.exists()) apico_path.mkdir((is_unix ? "downloads" : "Downloads"));
    return download_path.exists();
}

bool isAppPathExists()
{
    bool is_unix;
#ifdef Q_OS_UNIX
    is_unix = true;
#elif Q_OS_WINDOWS
    is_unix = false;
#endif
    QDir home_path = QDir::home();
    QDir apico_path = QDir(home_path.path()+QDir::separator()+(is_unix?".apico":"Apico"));
    if(!apico_path.exists()) home_path.mkdir((is_unix ? ".apico" : "Apico"));
    return apico_path.exists();
}


template<typename Func>
bool waitSignal(const typename QtPrivate::FunctionPointer<Func>::Object *sender, Func signal, int timeout)
{
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    QObject::connect(sender, signal, &loop, &QEventLoop::quit);
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(timeout);
    loop.exec();
    if(timer.isActive()) return true;
    else return false;
}

QString firstLetterToUp(QString str) {
    return (str.left(1).toUpper()+str.mid(1));
}
