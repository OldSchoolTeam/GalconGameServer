#-------------------------------------------------
#
# Project created by QtCreator 2011-06-01T18:18:56
#
#-------------------------------------------------

QT       += core network thread

QT       -= gui

TARGET = GalconGameServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
#    Game.cpp \
    NetworkController.cpp \
    Options.cpp \
    Player.cpp \
    Server.cpp \
    Session.cpp \
    Socket.cpp \
    ParsingException.cpp \
    Parser.cpp \
    Exception.cpp \
    ConnMsg.cpp \
    StepMsg.cpp \
    TimeToStartMsg.cpp \
    StateMsg.cpp \
    StartMsg.cpp \
    Fleet.cpp \
    FinishMsg.cpp \
    ErrMsg.cpp \
    ConnIdMsg.cpp \
    Planet.cpp

HEADERS += \
#    Game.h \
    NetworkController.h \
    Options.h \
    Player.h \
    Server.h \
    Session.h \
    Socket.h \
    StepMsg.h \
    ParsingException.h \
    Parser.h \
    Message.h \
    Exception.h \
    ConnMsg.h \
    Planet.h \
    Fleet.h \
    FinishMsg.h \
    ErrMsg.h \
    ConnIdMsg.h \
    TimeToStartMsg.h \
    StateMsg.h \
    StartMsg.h
