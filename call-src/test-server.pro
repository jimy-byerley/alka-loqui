
QT -= core gui

TARGET = test-server
INCLUDEPATH += ../portaudio-linux/include
SOURCES += utility.c call-server.c common.c test-server.c
HEADERS += utility.h call-server.h common.h
