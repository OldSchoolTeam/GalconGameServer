#-------------------------------------------------
#
# Project created by QtCreator 2011-06-06T23:21:38
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Game
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Planet.cpp \
    Game.cpp \
    Fleet.cpp \
    StepMsg.cpp \
    StateMsg.cpp

HEADERS += \
    Planet.h \
    Game.h \
    Fleet.h \
    StepMsg.h \
    StateMsg.h \
    Message.h
