QT -= core gui

TARGET = test-client
INCLUDEPATH += ../portaudio-linux/include
unix:LIBS += -lportaudio
SOURCES += utility.c call-client.c common.c test-client.c
HEADERS += utility.h call-client.h common.h
