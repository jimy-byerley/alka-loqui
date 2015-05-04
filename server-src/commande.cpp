#include "commande.h"


bool execCommand(QString text, Host user, QTcpSocket *socket)
{
    if(text[0]== QChar('/'))
    {
        QString commande;
        if(text.size()>1)
        {
            text=text.right(text.size()-1);
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
          ;
         }
        int nUser=Pseudo2Num(text);
              if( commande=="kick")
              {
                  if(permition(user))
                  {
                  if(pseudoExistant(cGuest[nUser],user)) //renvoie un message d'erreur si le pseudo n'existe pas (raison de la condition interne)
                    kick(cGuest[nUser]);
                  }
                  return false;
              }
              else if(commande=="ban")
              {
                  if(permition(user))
                  {
                  if(pseudoExistant(cGuest[nUser],user))
                     sentOne(ban(cGuest[nUser]),user);

                  }
                  return false;
              }
              else if(commande=="up")
              {
                  if(permition(user))
                  {
                  if(pseudoExistant(cGuest[nUser],user)&& cGuest[nUser].lvl<3)
                    cGuest[nUser].lvl+=1;

                  }
                  return false;
              }
              else if(commande=="down")
                  {
                    if(permition(user))
                    {
                      if(pseudoExistant(cGuest[nUser],user)&& cGuest[nUser].lvl>1)
                          cGuest[nUser].lvl-=1;

                    }
                    return false;
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
                  return false;
              }
              else if(commande=="pseudo")
               {
                  if(cGuest[nUser].lvl==-1)
                  {
                     if(user.lvl!=-1)
                     {
                        cGuest[Pseudo2Num(user.pseudo)].pseudo=text;
                        sentAll(user.pseudo+" c'est renommé en "+text);
                     }
                     else
                        sentAll(newClient(text,1,socket));
                     return false;
                  }
                  else
                  {
                      sentOne("Pseudo indisponible",user);
                      return false;
                  }
               }
              else if(commande=="list")
              {
                  QString list="";
                  for(int i = 1; i < cGuest.size(); i++)
                  {
                    list+=cGuest[i].pseudo+" ";
                  }
                  sentOne(list,user);
              }
              else
              {
                sentOne("Erreur commande inconnus",user);
                return false;
              }
        return false;
        }
        return true;
}
bool execCommandServ(QString text)
{
    QString commande="";
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
              p += text[i];
              if(i<(text.size()-1))
              i++;
          }
          if(i<text.size())
          {
              text=text.right(text.size()-(i+1));
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
             textSer->append(ban(cGuest[nUser]));
      }
      else if(commande=="unban")
      {
          textSer->append(unban(text));
      }
      else if(commande=="up" && cGuest[nUser].lvl<3)
      {
          if(pseudoExistantServ(cGuest[nUser]))
          {
              cGuest[nUser].lvl+=1;
              sentAll(cGuest[nUser].pseudo+" a etait up");
          }
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
      else if(commande=="list")
      {
          QString list="";
          for(int i = 1; i < cGuest.size(); i++)
          {
            list+=cGuest[i].pseudo+" ";
          }
          textSer->append(list);
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

bool permition(Host i)
{
    if(i.lvl>1)
        return true;
    else
    {
        sentOne("vous n'avais pas les droits",i);
        return false;
    }
}

void kick(Host user)
{
        sentAll("<i>"+user.pseudo+" a était kick</i>");
        user.socket->deleteLater();
        cGuest.removeOne(user);
}
QString unban(QString user)
{
    QFile blackList(BlackList);
    if(!blackList.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        return"erreur lors de l'ouverture de la blacklist";
    }
    QTextStream in(&blackList);
    int c=0;
    while (!in.atEnd())
    {
        QString pseudo="";
        QString line = in.readLine();
        int i=0;
        while(line[i]!=QChar(' ') and i <(line.size()-1))
        {
            pseudo+=line[i];
            if(i<(line.size()-1))
            i++;
        }
        if(pseudo==user)
        {
            in<<in.readAll().remove(c,c+line.size()-1);
            return pseudo+" a ete unban";
        }
        c++;
        //process_line(line);
    }
    return user+" n'a pas etait trouve dans la blacklist";
}

QString ban(Host user)
{
    QFile blackList(BlackList);
    if(!blackList.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        return"erreur lors de l'ouverture de la blacklist";
    }
    QTextStream out(&blackList);
    out<<blackList.readAll();
    out<<user.pseudo+" \n";
    //out<<user.socket->localAddress().toString()+"\n";
    sentAll("<serveur>"+user.pseudo+" a ete bannis");
    cGuest.removeOne(user);
    return "pour debannir "+user.pseudo+" utilise la fonction unban";
}
