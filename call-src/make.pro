
QT -= core gui

TARGET = call-server-test
INCLUDEPATH += ../portaudio-linux/include
SOURCES += utility.c call-server.c
HEADERS += utility.h call-server.h common.h