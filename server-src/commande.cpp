

#include "commande.h"
#include "client.h"
#include "fenserv.h"

bool execCommand(const QString &text, Host user,QTcpSocket *socket)
{
    if(text[0]=='/')
    {
        if(!(text.size==1))
        {
            text=text.substr(1,text.size()-1);
            QString commande;
            int i=0;
            while(text[i]!=' ' and i <(text.size()-1))
            {
                commande += text[i];
                i++;
            }
            if(i<text.size())
            {
                text=text.substr(i+1,text.size()-1);
            }
         }
        Host nUser=Pseudo2Host(text);
        switch(commande)
        {

              case "kick":
                  if(pseudoExistant(nUser)||permition(user.lvl))
                     kick(nUser);
                  break;
              case "ban":
                  if(pseudoExistant(nUser)||permition(user.lvl))
                     ban(nUser);
                  break;
              case "up":
                  if(pseudoExistant(nUser) || nUser.lvl<3 ||permition(user.lvl))
                    nUser.lvl+=1;
                  break;
             case "down":
                  if(pseudoExistant(nUser) || nUser.lvl>1 ||permition(user.lvl))
                     nUser.lvl-=1;
                  break;

             case "pseudo":
                if(user.lvl!=-1 || nUser.lvl==-1)
                    user.pseudo=text;
                else
                    newClient(text,1,user.socket);
                if(nUser.lvl!=-1)
                {
                    QByteArray paquet;
                    QDataStream out(&paquet, QIODevice::WriteOnly);

                    out << (quint16) 0;
                    out << tr("Pseudo indisponible");
                    out.device()->seek(0);
                    out << (quint16) (paquet.size() - sizeof(quint16));

                    socket->write(paquet);
                }
                break;
            default:
                QByteArray paquet;
                QDataStream out(&paquet, QIODevice::WriteOnly);

                out << (quint16) 0;
                out << tr("Erreur commande inconnue");
                out.device()->seek(0);
                out << (quint16) (paquet.size() - sizeof(quint16));

                socket->write(paquet);
                break;
        }
        return false;
     }
    else
    {
        return true;
    }

}

bool pseudoExistant(Host user)
{
    if(user.lvl!=-1)
        return true;
    else
        QByteArray paquet;
        QDataStream out(&paquet, QIODevice::WriteOnly);

        out << (quint16) 0;
        out << tr("Pseudo inconnus");
        out.device()->seek(0);
        out << (quint16) (paquet.size() - sizeof(quint16));

        socket->write(paquet);
        return false;
}

bool permition(char i)
{

}

void kick(Host user)
{
    FenServ.sentAll(tr("<strong>"+user.pseudo+"a était kick</strong>"));
    if(user!=host[0])
        host.removeOne(user);
}

void ban(Host user)
{

}
