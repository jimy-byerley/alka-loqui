#ifndef CLIENT_H
#define CLIENT_H

#include "QString"
#include <QTcpSocket>

typedef struct
{
    QString pseudo;
    char lvl;
    QTcpSocket *socket;

}Host;

bool operator==(Host const& a, Host const& b);
QString newClient(QString pseudo, const char lvl, QTcpSocket *newGuest, QList<Host> *cGuest);
Host Pseudo2Host(QString pseudo, QList<Host> cGuest);
Host Socket2Client(QTcpSocket *socket,QList<Host> cGuest);


#endif // CLIENT_H
