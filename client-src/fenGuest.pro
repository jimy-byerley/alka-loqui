#-------------------------------------------------
#
# Project created by QtCreator 2015-04-10T10:40:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = fenGuest
TEMPLATE = app
INCLUDEPATH += ../portaudio/include
LIBPATH += ../portaudio/lib/win32:LIBS += portaudio_x86.dll

SOURCES += main.cpp\
    fenguest.cpp\
../call-src/call-client.c\
../call-src/common.c\
../call-src/utility.c\



HEADERS  += \
    fenguest.h\
.../call-src/call-client.h\
../call-src/common.h\
../call-src/utility.h\
../portaudio/include/portaudio.h\
