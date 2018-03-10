#ifndef EXTS_FUNCS_H
#define EXTS_FUNCS_H

#include <QDir>
#include <QEventLoop>
#include <QTimer>
#include <QUrl>

bool isDownloaderPathExists();

bool isAppPathExists();

template<typename Func>
bool waitSignal(const typename QtPrivate::FunctionPointer<Func>::Object *sender, Func signal, int timeout = 30000);

QString firstLetterToUp(QString str);

QUrl convertGithubUrlToApiReq(QUrl url, bool isOrganisation = false);

#endif // EXTS_FUNCS_H
