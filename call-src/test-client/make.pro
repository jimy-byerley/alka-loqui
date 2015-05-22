QT -= core gui

TARGET = test-client
INCLUDEPATH += ../../portaudio/include
unix:LIBPATH     += ../../portaudio/lib/x86_64-linux-gnu
win32:LIBPATH     += ../../portaudio/lib/win32
LIBS   += -lportaudio
SOURCES     += ../utility.c ../call-client.c ../common.c test-client.c
HEADERS     += ../utility.h ../call-client.h ../common.h
CONFIG      += debug
QMAKE_CFLAGS_DEBUG = -ggdb
QMAKE_CFLAGS = -Wno-unused-value
