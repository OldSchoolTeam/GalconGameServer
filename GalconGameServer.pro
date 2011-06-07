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
    StepMsg.cpp \
    ParsingException.cpp \
    ConnMsg.cpp \
    Exception.cpp \
    Parser.cpp

HEADERS += \
#    Game.h \
    NetworkController.h \
    StepMsg.h \
    ParsingException.h \
    ConnMsg.h \
    Options.h \
    Player.h \
    Server.h \
    Session.h \
    Socket.h \
    Message.h \
    Exception.h \
    Parser.h
