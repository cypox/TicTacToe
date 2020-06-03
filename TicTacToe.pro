#-------------------------------------------------
#
# Project created by QtCreator 2013-12-16T11:17:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TicTacToe
TEMPLATE = app


SOURCES += main.cpp\
	gamewindow.cpp \
    minmax.cpp \
    config.cpp

HEADERS  += gamewindow.h \
    minmax.h \
    config.h

FORMS    += gamewindow.ui
