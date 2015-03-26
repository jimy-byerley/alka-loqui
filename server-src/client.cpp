#include "client.h"

bool operator==(Host const& a, Host const& b)
{
    if (a.lvl == b.lvl && a.pseudo == b.pseudo && a.socket == b.socket)
        return true;
    else
        return false;
}

void newClient(QString pseudo, const char lvl, QTcpSocket *newGuest)
{
    Host nClient;
    nClient.lvl=lvl;
    nClient.pseudo=pseudo;
    nClient.socket=newGuest;
    host << nClient;
}
Host Pseudo2Host(QString pseudo)
{
    int i=0;
    while(host[i].pseudo!=pseudo && i<host.size())
    {
        i++;
    }
    if(i<host.size())
        return host[i];
    else
        return host[0];
}
Host Socket2Client(QTcpSocket *socket)
{
    int i=0;
    while(host[i].socket!=socket && i<host.size())
    {
        i++;
    }
    if(i<host.size())
        return host[i];
    else
        return host[0];
}
