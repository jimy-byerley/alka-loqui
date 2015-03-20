#include "mainserveur.h"

void newCon()
    {
        QTcpSocket *newGuest = server->nextPendingConnection();
        guests<< newGuest;
        connect(newGuest, SIGNAL(readyRead()), this, SLOT(dataRec()));
        connect(newGuest, SIGNAL(disconnected()), this, SLOT(discGuest()));
    }
void dataRec()
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

        Host user=Socket2Client(socket);
        QString text;
        paqEnt >> text;
        if(execCommand(text,user,socket)) //Verification et execution de commande                 //modif
        {
            sentAll(text);
        }
        tailleMessage=0;
    }
void discGuest()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (socket == 0)
        return;
    Host user=Socket2Client(socket);                                                  //modif
    if(user.lvl!=-1)
        sentAll(tr("<strong>"+user.pseudo+" nous a quitté</strong>"));
        host.removeOne(user);
        guests.removeOne(socket);
    socket->deleteLater();
}
void sentAll(const QString &text)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << text;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    for (int i = 0; i < host.size(); i++)
    {
        host[i].socket->write(paquet);                                                //modif
    }

}
