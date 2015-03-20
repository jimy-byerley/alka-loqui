#ifndef CLIENT_H
#define CLIENT_H

#include "QString"
#include "QTcpSocket"


typedef struct
{
    QString pseudo;
    char lvl;
    QTcpSocket *socket;

}Host;

void newClient(QString pseudo,const char lvl,QTcpSocket newGuest);
Host Pseudo2Host(QString pseudo);
Host Socket2Client(QTcpSocket *socket);
QList<Host> host;


#endif // CLIENT_H
