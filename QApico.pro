#-------------------------------------------------
#
# Project created by QtCreator 2018-02-06T10:04:27
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QApico
TEMPLATE = app

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    downloader.cpp \
    database.cpp \
    exts_funcs.cpp \
    parser.cpp \
    parserresult.cpp

HEADERS += \
        mainwindow.h \
    core.h \
    downloader.h \
    database.h \
    exts_funcs.h \
    parser.h \
    parserresult.h \
    database_names.h \
    enums.h \
    taskresult.h

FORMS += \
        mainwindow.ui

SUBDIRS += libs/QGumboParser/QGumboParser

unix|win32: LIBS += -L$$PWD/libs/QGumboParser/QGumboParser/ -lQGumboParser

INCLUDEPATH += $$PWD/libs/QGumboParser/QGumboParser
DEPENDPATH += $$PWD/libs/QGumboParser/QGumboParser

LIBS += -lsqlite3

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/QGumboParser/QGumboParser/QGumboParser.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/libs/QGumboParser/QGumboParser/libQGumboParser.a

#INCLUDEPATH += $$[QT_INSTALL_PREFIX]/src/3rdparty/sqlite
#SOURCES += $$[QT_INSTALL_PREFIX]/src/3rdparty/sqlite/sqlite3.c
