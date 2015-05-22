#-------------------------------------------------
#
# Project created by QtCreator 2015-02-17T13:00:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = FenServeur
TEMPLATE = app
INCLUDEPATH      += . ../portaudio/include
unix:LIBPATH     += ../portaudio/lib/x86_64-linux-gnu
win32:LIBPATH    += ../portaudio/lib/win32

SOURCES += main.cpp\
	fenServ.cpp\
	commande.cpp \
	client.cpp \
	mainserveur.cpp

HEADERS  += fenserv.h commande.h client.h mainserveur.h

# call-src
SOURCES += ../call-src/call-client.c ../call-src/common.c ../call-src/utility.c
HEADERS += ../call-src/call-client.h ../call-src/common.h ../call-src/utility.h
