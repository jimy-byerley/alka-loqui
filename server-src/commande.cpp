

#include "commande.h"
#include "Client.h"
#include "fenserv.h"

execCommand(const QString &text)
{
    if(text[0]=='/')
    {
        if(!(text.size==1))
        {
            text=text.substr(1,text.size()-1);
            QString commande;
            int i=0;
            while(!(text[i]==' ') and i <(text.size()-1))
            {
                commande += text[i];
                i++;
            }
            if(i<text.size())
            {
                text=text.substr(i+1,text.size()-1);
            }
         }
        switch(commande)
        {
            case "kick":
                kick(text);
                break;
            case "ban":
                ban(text);
                break;
            case "up"
                //a voir avec le lvl client
                break;
            case "down"
                //idem
                break;
            default:
                QByteArray paquet;
                QDataStream out(&paquet, QIODevice::WriteOnly);

                out << (quint16) 0;
                out << "Erreur commande inconnue";
                out.device()->seek(0);
                out << (quint16) (paquet.size() - sizeof(quint16));

                guests[n]->write(paquet);
                break;
        }
     }
    else
    {
        FenServ.sentAll(text);
    }

}


void kick(QString user)
{
    nUser=FenServ.list(user);
    FenServ.sentAll(tr("<strong>"+user+"a était kick</strong>"));
    FenServ.remove(nUser);
}

void ban(QString user)
{

}
