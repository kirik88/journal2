#-------------------------------------------------
#
# Project created by QtCreator 2013-02-11T12:48:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = journal2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    journals.cpp \
    journal.cpp \
    loader.cpp

HEADERS  += mainwindow.h \
    journals.h \
    journal.h \
    loader.h

FORMS    += mainwindow.ui
