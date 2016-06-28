#-------------------------------------------------
#
# Project created by QtCreator 2016-02-22T12:09:00
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IBSampleQt
TEMPLATE = app

# NKTP Interbus project include
include(Interbus/Interbus.pri)

SOURCES += main.cpp\
        mainwindow.cpp \

HEADERS  += mainwindow.h \

FORMS    += mainwindow.ui
