
QT -= core gui

TARGET = test-server
#INCLUDEPATH += ../portaudio-linux/include
#DEPENDPATH += ../portaudio-linux/lib/x86_64-linux-gnu
SOURCES += utility.c call-server.c common.c test-server.c
HEADERS += utility.h call-server.h common.h
