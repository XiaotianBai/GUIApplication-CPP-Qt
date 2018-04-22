#-------------------------------------------------
#
# Project created by QtCreator 2016-03-17T15:02:03
#
#-------------------------------------------------
QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Arm_Controller
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    mythread.cpp \

HEADERS  += mainwindow.h \
    mythread.h \
    myStruct.h \

RESOURCES += \
    images/images.qrc
    images\images.qrc
FORMS += \
    mainwindow.ui
RC_FILE = app.rc


