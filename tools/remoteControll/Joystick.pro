#-------------------------------------------------
#
# Project created by QtCreator 2013-05-21T20:22:15
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Joystick
TEMPLATE = app


SOURCES += main.cpp \
    jmainwindow.cpp \
    jjoystick.cpp \
    jdirectionalpad.cpp \
    jbotsimulator.cpp \
    jvector.cpp \
    jfieldview.cpp \
    jtcpmanager.cpp

HEADERS  += \
    jmainwindow.h \
    jjoystick.h \
    jdirectionalpad.h \
    jbotsimulator.h \
    jvector.h \
    jfieldview.h \
    jtcpmanager.h

FORMS    += \
    jmainwindow.ui

unix|win32: LIBS += -lSDL
