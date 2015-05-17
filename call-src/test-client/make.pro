QT -= core gui

TARGET = test-client
INCLUDEPATH += ../../portaudio-linux/include
LIBPATH     += ../../portaudio-linux/lib/x86_64-linux-gnu
unix:LIBS   += -lportaudio
SOURCES     += ../utility.c ../call-client.c ../common.c test-client.c
HEADERS     += ../utility.h ../call-client.h ../common.h
CONFIG      += debug
QMAKE_CFLAGS_DEBUG = -ggdb
QMAKE_CFLAGS = -Wno-unused-value
