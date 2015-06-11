#-------------------------------------------------
#
# Project created by QtCreator 2013-03-21T11:15:18
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Assignment1
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    table.cpp \
    ball.cpp \
    pocket.cpp \
    tableitem.cpp \
    block.cpp \
    ballfactory.cpp \
    physicsprototype.cpp \
    residue.cpp \
    blackhole.cpp

HEADERS  += dialog.h \
    table.h \
    ball.h \
    pocket.h \
    tableitem.h \
    block.h \
    ballfactory.h \
    physicsprototype.h \
    residue.h \
    blackhole.h

FORMS    += dialog.ui

RESOURCES += \
    sounds.qrc

OTHER_FILES += \
    config.txt

