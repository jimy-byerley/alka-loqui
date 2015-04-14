#ifndef COMMANDE_H
#define COMMANDE_H

#include "mainserveur.h"
#include "client.h"
#include "fenserv.h"

#include <QtWidgets>
#include <QtNetwork>
#include <QtDebug>
#include <QDebug>



bool execCommand(QString text, Host user, QTcpSocket *socket); //condition:'/' pour le depart d'une fonction,departition entre les fonction
bool execCommandServ(QString text);
bool pseudoExistantServ(Host user);
QString ban(Host user);
QString unban(QString user);
void kick(Host user);
bool pseudoExistant(Host user, Host requet);
bool permition(char i);

#endif
