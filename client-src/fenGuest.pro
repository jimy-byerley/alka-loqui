#-------------------------------------------------
#
# Project created by QtCreator 2015-04-10T10:40:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET           = fenGuest
TEMPLATE         = app
INCLUDEPATH      += ../portaudio/include
unix:LIBPATH     += ../portaudio/lib/x86_64-linux-gnu
win32:LIBPATH    += ../portaudio/lib/win32

SOURCES += main.cpp fenguest.cpp
HEADERS += fenguest.h

SOURCES += ../call-src/call-client.c ../call-src/common.c ../call-src/utility.c
HEADERS += ../call-src/call-client.h ../call-src/common.h ../call-src/utility.h
