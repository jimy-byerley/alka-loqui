#ifndef COMMANDE_H
#define COMMANDE_H
#include "mainserveur.h"
#include "client.h"


#include <QtWidgets>
#include <QtNetwork>



bool execCommand(const QString text, Host user, QTcpSocket *socket, QList<Host> cGuest); //condition:'/' pour le depart d'une fonction,departition entre les fonction
void ban(Host user);
void kick(Host user, QList<Host> cGuest);
bool pseudoExistant(Host user, Host requet);
bool permition(char i);

#endif
