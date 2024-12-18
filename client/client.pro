#-------------------------------------------------
#
# Project created by QtCreator 2024-12-05T10:39:55
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        client.cpp \
    front.cpp

HEADERS  += client.h \
    front.h

FORMS    += client.ui \
    front.ui

RESOURCES += \
    image.qrc
