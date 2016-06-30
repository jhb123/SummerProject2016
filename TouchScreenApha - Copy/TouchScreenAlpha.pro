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
    touchscreen.cpp \
    edfa.cpp \
    laser.cpp \
    lasersystem.cpp

HEADERS  += \
    touchscreen.h \
    edfa.h \
    laser.h \
    lasersystem.h

FORMS    += \
    touchscreen.ui

RESOURCES += \
    turnon1.qrc \
    turnon1.qrc
