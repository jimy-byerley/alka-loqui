#-------------------------------------------------
#
# Project created by QtCreator 2015-02-17T13:00:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = FenServeur
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .

SOURCES += main.cpp\
        fenServ.cpp\
        commande.cpp \
    client.cpp \
    mainserveur.cpp


HEADERS  += fenserv.h\
         commande.h \
    client.h \
    mainserveur.h


