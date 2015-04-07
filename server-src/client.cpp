#include "client.h"

QList<Host> cGuest;

bool operator==(Host const& a, Host const& b)
{
    if (a.lvl == b.lvl && a.pseudo == b.pseudo && a.socket == b.socket)
        return true;
    else
        return false;
}

QString newClient(QString pseudo, const char lvl, QTcpSocket *newGuest)
{
    Host nClient;
    nClient.lvl=lvl;
    nClient.pseudo=pseudo;
    nClient.socket=newGuest;
    nClient.tailleMessage=0;
    cGuest.append(nClient);
    return "<strong>"+pseudo+" nous a quitté</strong>";
}
Host Pseudo2Host(QString pseudo)
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
int Pseudo2Num(QString pseudo)
{
    int i=0;
    qDebug("pseudo size:%d",pseudo.size());
    while(cGuest[i].pseudo!=pseudo && i<cGuest.size()-1)
    {
        if(i<cGuest.size()-1)
        i++;
        qDebug("i:%d",i);
    }
    if(i<=cGuest.size()-1 && cGuest[i].pseudo==pseudo)
        return i;
    else
        return 0;
}

Host Socket2Client(QTcpSocket *socket)
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
