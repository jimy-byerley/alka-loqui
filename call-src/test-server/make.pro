
QT -= core gui

TARGET = test-server
INCLUDEPATH += ../../portaudio-linux/include
LIBPATH += ../../portaudio-linux/lib/x86_64-linux-gnu
SOURCES += ../utility.c ../call-server.c ../common.c test-server.c
HEADERS += ../utility.h ../call-server.h ../common.h
CONFIG += debug
QMAKE_CFLAGS_DEBUG = -ggdb
QMAKE_CFLAGS = -Wno-unused-value
