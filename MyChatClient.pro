#-------------------------------------------------
#
# Project created by QtCreator 2017-07-01T22:38:15
#
#-------------------------------------------------

QT += core gui
QT += network
QT += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyChatClient
TEMPLATE = app


SOURCES += main.cpp\
        json_worker.cpp \
        widget.cpp

HEADERS  += widget.h \
    json_worker.h

FORMS    += widget.ui
