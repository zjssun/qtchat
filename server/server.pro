#-------------------------------------------------
#
# Project created by QtCreator 2024-12-05T10:40:26
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app


SOURCES += main.cpp\
        socket.cpp \
        widget.cpp \
        worker.cpp \
    mytcp.cpp

HEADERS  += widget.h \
    socket.h \
    worker.h \
    mytcp.h

FORMS    += widget.ui

RESOURCES += \
    image.qrc
