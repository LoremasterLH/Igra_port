#-------------------------------------------------
#
# Project created by QtCreator 2014-09-08T11:17:01
#
#-------------------------------------------------

QT       += core gui \
    xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Igra
TEMPLATE = app

SOURCES += main.cpp\
        game.cpp \
    unit.cpp \
    field.cpp \
    board.cpp \
    settings.cpp

HEADERS  += game.h \
    unit.h \
    field.h \
    board.h \
    settings.h \
    colors.h

FORMS    += game.ui \
    settings.ui

DEFINES *= QT_USE_QSTRINGBUILDER
