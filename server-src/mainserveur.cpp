#include "mainserveur.h"

mainserveur::mainserveur()
{
    server = new QTcpServer(this);
    //Gérance du serveur
    tailleMessage = 0;
    Host defaut;
    defaut.lvl=-1;
    cGuest<<defaut;
    Host poney;             //je suis là pour les tests
    poney.lvl=1;
    poney.pseudo="jack";
    cGuest<<poney;
}

QString mainserveur::demarage()
{
    QString servStat;
    if(!server->listen(QHostAddress::Any, 42424)) //Tout adresse IP et sur port 42424
    {
        servStat="Problème de démarrage serveur dû à <br />" + server->errorString();
    }
    else
    {
        servStat="Démarrage accompli avec succès sur le port <strong>"+ QString::number(server->serverPort()) +"</strong>.<br />Les clients peuvent se connecter";
        connect(server, SIGNAL(newConnection()),this, SLOT(newCon()));
    }
    return servStat;
}

void mainserveur::newCon()
    {
        QTcpSocket *newGuest = server->nextPendingConnection();
        nCo nGuest={newGuest,0};
        guests<< nGuest;                //modif importante
        connect(newGuest, SIGNAL(disconnected()),this ,SLOT(discGuest()));
        connect(newGuest, SIGNAL(readyRead()),this , SLOT(dataRec()));

    }

void mainserveur::dataRec()
    {
        QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
        if (socket == 0)
            return;
        int i=found_nCo(socket);
        QDataStream paqEnt(socket);

        if (guests[i].tailleMessage == 0)
        {
            if (socket->bytesAvailable() < (int)sizeof(quint16))
                return;

            paqEnt >> guests[i].tailleMessage;                          //modif importante
        }

        if (socket->bytesAvailable() < guests[i].tailleMessage)
            return;

        Host user=Socket2Client(socket);
        QString text;
        paqEnt >> text;
        if(user.lvl!=-1 && execCommand(text,user,socket)) //Verification et execution de commande
        {

            text="b" + user.pseudo + "</b> : "+text;
            sentAll(text);
//            pthread_t t;
//            pthread_create(&t,NULL,sentAll,*text)
        }
        guests[i].tailleMessage=0;
    }
void mainserveur::discGuest()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (socket == 0)
        return;
    Host user=Socket2Client(socket);
    if(user.lvl!=-1)
    {
        sentAll("<strong>"+user.pseudo+" nous a quitté</strong>");
        cGuest.removeOne(user);
    }
    guests.removeOne(guests[found_nCo(socket)]);
    socket->deleteLater();
}

int mainserveur::found_nCo(QTcpSocket *a)
{
    int i=0;
    while(guests[i].socket!=a)
    {
        i++;
    }
    return i;
}
bool operator==(nCo a,nCo b)
{
    if(a.socket==b.socket)
        return true;
    return false;
}

void sentAll(QString text)
{
    time_t secondes;
    struct tm instant;

    time(&secondes);
    instant=*localtime(&secondes);
    QString heure="<"+QString::number(instant.tm_hour)+":"+QString::number(instant.tm_min)+":"+QString::number(instant.tm_sec)+">";
    text=heure+" "+text;

    textSer->append(text);  //affiche sur la fenetre du serveur

    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << text;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));
    qDebug("initialisé i=0 pas a 1");
    for (int i = 1; i < cGuest.size()-1; i++)                                           //initialisé i=0
    {
        cGuest[i].socket->write(paquet);                                                //modif
    }
}
void sentOne(QString text, Host user)
{
    time_t secondes;
    struct tm instant;

    time(&secondes);
    instant=*localtime(&secondes);

    QString heure=QString::number(instant.tm_hour)+":"+QString::number(instant.tm_min)+":"+QString::number(instant.tm_sec);
    text=heure+" "+text;

    textSer->append(text);

    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << text;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    user.socket->write(paquet);
}

