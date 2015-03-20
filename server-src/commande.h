#ifndef COMMANDE_H
#define COMMANDE_H
#include "client.h"
#include <QString>



bool execCommand(const QString &text, Host user, QTcpSocket *socket); //condition:'/' pour le depart d'une fonction,departition entre les fonction
void ban(Host user);
void kick(Host user);
bool PseudoExistant(Host user);
bool permition(char i);

#endif
