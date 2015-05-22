
QT -= core gui

TARGET = test-server
INCLUDEPATH += ../../portaudio-linux/include
unix:LIBPATH     += ../../portaudio/lib/x86_64-linux-gnu
win32:LIBPATH     += ../../portaudio/lib/win32
# le serveur ne necessite pas d'etre lié a la bibliotheque (car n'utilise par l'interface son
#LIBS   += -lportaudio
SOURCES += ../utility.c ../call-server.c ../common.c test-server.c
HEADERS += ../utility.h ../call-server.h ../common.h
CONFIG += debug
QMAKE_CFLAGS_DEBUG = -ggdb
QMAKE_CFLAGS = -Wno-unused-value
