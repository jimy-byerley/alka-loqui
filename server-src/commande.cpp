#include "commande.h"


bool execCommand(QString text, Host user)
{
    if(text.data()[0]=='/')
    {
        QString commande;
        if(text.size()!=1)
        {
            text=text.right(2);
            int i=0;
            while(text.data()[i]!=' ' and i <(text.size()-1))
            {
                commande += text.data()[i];
                i++;
            }
            if(i<text.size())
            {
                text=text.right(i+1);
            }
         }
        Host nUser=Pseudo2Host(text);

              if( commande=="kick"&& pseudoExistant(nUser,user)&&permition(user.lvl))
                kick(nUser);
              else if(commande=="ban"&& pseudoExistant(nUser,user)&&permition(user.lvl))
                ban(nUser);
              else if(commande=="up" && pseudoExistant(nUser,user) && nUser.lvl<3 &&permition(user.lvl))
                nUser.lvl+=1;
              else if(commande=="down" &&pseudoExistant(nUser,user) && nUser.lvl>1 &&permition(user.lvl))
                nUser.lvl-=1;
              else if(commande=="pseudo")
               {
                  if(nUser.lvl==-1)
                  {
                     if(user.lvl!=-1)
                        user.pseudo=text;
                     else
                        newClient(text,1,user.socket);
                  }
                  else
                  {
                      sentOne("Pseudo indisponible",user);
                  }
               }
              else
              {
                sentOne("Erreur commande inconnus",user);
              }
        return false;
        }
        return true;
     }


bool pseudoExistant(Host user,Host requet)
{
    if(user.lvl!=-1)
        return true;
    else
    {
        sentOne("Pseudo inconnus",requet);
        return false;
    }
}

bool permition(char i)
{
    if(i>1)
        return true;
    else
        return false;
}

void kick(Host user)
{
    sentAll("<strong>"+user.pseudo+"a était kick</strong>");
    if(user.lvl!=-1)
        host.removeOne(user);
}

void ban(Host user)
{
    user.lvl;
}
