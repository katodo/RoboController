#-------------------------------------------------
#
# Project created by QtCreator 2013-04-20T11:36:43
#
#-------------------------------------------------

QT       += core gui printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RoboController-PID_Regulator
TEMPLATE = app

ROBOCONTROLLERSDKPATH = ../RoboControllerSDK

DEFINES += ROBOCONTROLLERSDK_LIBRARY

include($$ROBOCONTROLLERSDKPATH/mod_CORE/RoboControllerSDK_CORE.pri) # Core module
include($$ROBOCONTROLLERSDKPATH/mod_GUI/RoboControllerSDK_GUI.pri) # GUI module

INCLUDEPATH += \
        ../common/3rdparty/qcustomplot/include/ \
        ../common/include/

SOURCES += \
        main.cpp\
        cmainwindow.cpp \        
        cselectipdlg.cpp \
        ../common/3rdparty/qcustomplot/src/qcustomplot.cpp \
        cconfiginidialog.cpp

HEADERS  += \
    cmainwindow.h \    
    macros.h \
    cselectipdlg.h \
    ../common/3rdparty/qcustomplot/include/qcustomplot.h \
    cconfiginidialog.h

FORMS    += \
    cmainwindow.ui \
    cselectipdlg.ui \
    cconfiginidialog.ui
