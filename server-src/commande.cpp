#include "commande.h"


bool execCommand(QString text, Host user, QTcpSocket *socket)
{
    if(text[0]== QChar('/'))
    {
        QString commande;
        if(text.size()>1)
        {
            text=text.right(1);
            int i=0;
            while(i <(text.size()-1) && text[i]!= QChar(' '))
              {
                  commande += text[i];
                  if(i<(text.size()-1))
                      i++;
              }
            if(i<text.size())
            {
                text=text.right(text.size()-(i+1));
            }
         }
        int nUser=Pseudo2Num(text);
        bool pseudoEx = pseudoExistant(cGuest[nUser],user);

              if( commande=="kick" &&permition(user.lvl))
              {
                  if(pseudoEx)
                    kick(cGuest[nUser]);
              }
              else if(commande=="ban"&&permition(user.lvl))
              {
                  if(pseudoEx)
                     ban(cGuest[nUser]);
              }
              else if(commande=="up"&& cGuest[nUser].lvl<3 &&permition(user.lvl))
              {
                  if(pseudoEx)
                    cGuest[nUser].lvl+=1;
              }
              else if(commande=="down"&& cGuest[nUser].lvl>1 &&permition(user.lvl))
                  {
                      if(pseudoEx)
                          cGuest[nUser].lvl-=1;
                  }
              else if(commande=="mp")
              {
                  int i=0;
                  QString p;
                  while(text[i]!=QChar(' ') and i <(text.size()-1))
                  {
                      p += text[i];
                      if(i <(text.size()-1))
                      i++;
                  }
                  if(i<text.size())
                  {
                      text=text.right(i+1);
                  }
                  if(pseudoExistant(cGuest[Pseudo2Num(p)],user))
                  {
                    text=user.pseudo+":"+text;
                    sentOne(text,cGuest[Pseudo2Num(p)]);
                  }
                  else
                    sentOne("Pseudo inconnue",user);
              }
              else if(commande=="pseudo")
               {
                  if(cGuest[nUser].lvl==-1)
                  {
                     if(user.lvl!=-1)
                        user.pseudo=text;
                     else
                        sentAll(newClient(text,1,socket));
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
bool execCommandServ(QString text)
{
    QString commande="";
    qDebug("%d",text.size());
    int i=0;
    while(i <(text.size()-1) && text[i]!= QChar(' '))
      {
          commande += text[i];
          if(i<(text.size()-1))
              i++;
      }
    if(i<text.size())
    {
        text=text.right(text.size()-(i+1));
    }

    int nUser=Pseudo2Num(text);
    qDebug("nUser=%d",nUser);
      if( commande=="kick")
      {
          if(pseudoExistantServ(cGuest[nUser]))
            kick(cGuest[nUser]);
      }
      else if (commande=="say")
      {
          text="[serveur]:"+text;
          sentAll(text);
      }
      else if(commande=="mp")
      {
          QString p;
          int i=0;
          while(text[i]!=QChar(' ') and i <(text.size()-1))
          {
              qDebug("i:%d",i);
              p += text[i];
              if(i<(text.size()-1))
              i++;
          }
          if(i<text.size())
          {
              text=text.right(i+1);
          }
          if(pseudoExistantServ(cGuest[Pseudo2Num(p)]))
          {
            text="[mp-serveur]:"+text;
            sentOne(text,cGuest[Pseudo2Num(p)]);
          }
          else
          {
              return false;
          }
      }
      else if(commande=="ban")
      {
          if(pseudoExistantServ(cGuest[nUser]))
            ban(cGuest[nUser]);
      }
      else if(commande=="up" && cGuest[nUser].lvl<3)
      {
          if(pseudoExistantServ(cGuest[nUser]))
            cGuest[nUser].lvl+=1;
      }
      else if(commande=="down" && cGuest[nUser].lvl>1)
      {
          if(pseudoExistantServ(cGuest[nUser]))
            cGuest[nUser].lvl-=1;
      }
      else if(commande=="stop")
      {
          //en reflexion
      }
      else
      {
        textSer->append("Erreur commande inconnus");
        return false;
      }  
      return true;
}

bool pseudoExistantServ(Host user)
{
    if(user.lvl!=-1)
        return true;
    else
    {
        textSer->append("Pseudo inconnus");
        return false;
    }
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
        sentAll("<i>"+user.pseudo+" a était kick</i>");
        cGuest.removeOne(user);
}

void ban(Host user)
{
    user.lvl;
}
