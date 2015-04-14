#ifndef CLIENT_H
#define CLIENT_H

#include "QString"
#include <QTcpSocket>

typedef struct
{
    QString pseudo;
    char lvl;
    QTcpSocket *socket;
    quint16 tailleMessage;

}Host;
extern QList<Host> cGuest;

bool operator==(Host const& a, Host const& b);
QString newClient(QString pseudo, const char lvl, QTcpSocket *newGuest);
Host Pseudo2Host(QString pseudo);
Host Socket2Client(QTcpSocket *socket); //inicialise
int Pseudo2Num(QString pseudo);


#endif // CLIENT_H
