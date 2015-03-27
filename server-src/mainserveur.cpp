#include "mainserveur.h"

mainserveur::mainserveur()
{
    server = new QTcpServer(this);
    //Gérance du serveur
    tailleMessage = 0;
    Host defaut;
    defaut.lvl=-1;
    cGuest<<defaut;
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
        guests<< newGuest;
        connect(newGuest, SIGNAL(disconnected()),this ,SLOT(discGuest()));
        connect(newGuest, SIGNAL(readyRead()),this , SLOT(dataRec()));

    }

void mainserveur::dataRec()
    {

        QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
        if (socket == 0)
            return;

        QDataStream paqEnt(socket);

        if (tailleMessage == 0)
        {
            if (socket->bytesAvailable() < (int)sizeof(quint16))
                return;

            paqEnt >> tailleMessage;
        }

        if (socket->bytesAvailable() < tailleMessage)
            return;

        Host user=Socket2Client(socket,cGuest);
        QString text;
        paqEnt >> text;
        if(user.lvl!=-1 && execCommand(text,user,socket,cGuest)) //Verification et execution de commande                 //modif
        {

            text="b" + user.pseudo + "</b> : "+text;
            sentAll(text,cGuest);
        }
        tailleMessage=0;
    }
void mainserveur::discGuest()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (socket == 0)
        return;
    Host user=Socket2Client(socket,cGuest);                                                  //modif
    if(user.lvl!=-1)
    {
        sentAll("<strong>"+user.pseudo+" nous a quitté</strong>",cGuest);
        cGuest.removeOne(user);
    }
    guests.removeOne(socket);
    socket->deleteLater();
}
void sentAll(const QString &text,QList<Host> cGuest)
{
    time_t secondes;
    struct tm instant;

    time(&secondes);
    instant=*localtime(&secondes);

    QString heure=QString::number(instant.tm_hour)+":"+QString::number(instant.tm_min)+":"+QString::number(instant.tm_sec);
    text=heure+" "+text;

    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << text;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    for (int i = 0; i < cGuest.size(); i++)
    {
        cGuest[i].socket->write(paquet);                                                //modif
    }

}
void sentOne(const QString &text,Host user)
{
    time_t secondes;
    struct tm instant;

    time(&secondes);
    instant=*localtime(&secondes);

    QString heure=QString::number(instant.tm_hour)+":"+QString::number(instant.tm_min)+":"+QString::number(instant.tm_sec);
    text=heure+" "+text;

    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << text;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    user.socket->write(paquet);
}
