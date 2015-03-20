#include "serveur.h"

void serveur()
{

}
void newCon()
{
	sentAll(tr("<strong>Quelqu'un vient de se connecter.</strong>"));

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


        QString text;
        paqEnt >> text;
        if(execCommand(text)) //Verification et execution de commande
        {
            sentAll(text);
        }
        tailleMessage=0;
}
void discGest()
{
    sentAll(tr("<strong>quelqu'un nous a quitté</strong>"));

    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (socket == 0)
        return;

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

    for (int i = 0; i < guests.size(); i++)
    {
        guests[i]->write(paquet);
    }
}