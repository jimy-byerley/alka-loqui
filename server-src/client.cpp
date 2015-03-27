#include "client.h"


bool operator==(Host const& a, Host const& b)
{
    if (a.lvl == b.lvl && a.pseudo == b.pseudo && a.socket == b.socket)
        return true;
    else
        return false;
}

QString newClient(QString pseudo, const char lvl, QTcpSocket *newGuest, QList<Host> *cGuest)
{
    Host nClient;
    nClient.lvl=lvl;
    nClient.pseudo=pseudo;
    nClient.socket=newGuest;
    cGuest->append(nClient);
    return "<strong>"+pseudo+" nous a quitté</strong>";
}
Host Pseudo2Host(QString pseudo, QList<Host> cGuest)
{
    int i=0;
    while(cGuest[i].pseudo!=pseudo && i<cGuest.size())
    {
        i++;
    }
    if(i<cGuest.size())
        return cGuest[i];
    else
        return cGuest[0];
}
Host Socket2Client(QTcpSocket *socket, QList<Host> cGuest)
{
    int i=0;
    while(cGuest[i].socket!=socket && i<cGuest.size())
    {
        i++;
    }
    if(i<cGuest.size())
        return cGuest[i];
    else
        return cGuest[0];
}
