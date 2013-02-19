#-------------------------------------------------
#
# Project created by QtCreator 2013-02-11T12:48:25
#
#-------------------------------------------------

QT       += core gui    \
    xml \
    network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = journal2
TEMPLATE = app

INCLUDEPATH += .

SOURCES += main.cpp\
        mainwindow.cpp \
    journals.cpp \
    journal.cpp \
    loader.cpp \
    user.cpp \
    answer.cpp \
    answerenums.cpp \
    loaderenums.cpp \
    enums.cpp \
    userenums.cpp \
    journallist.cpp \
    column.cpp \
    journaltablewidget.cpp

HEADERS  += mainwindow.h \
    journals.h \
    journal.h \
    loader.h \
    user.h \
    answer.h \
    journallist.h \
    column.h \
    journaltablewidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    journal2.qrc

RC_FILE += \
    journal2.rc
