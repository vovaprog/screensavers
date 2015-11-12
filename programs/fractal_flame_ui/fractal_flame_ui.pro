#-------------------------------------------------
#
# Project created by QtCreator 2015-11-08T10:26:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fractal_flame_ui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS += -L../fractal_flame/build -lfractal_flame_library -ltinyxml -lboost_filesystem -lboost_system -lfreeimage


INCLUDEPATH += ../fractal_flame ../../mylib

CONFIG += c++11
