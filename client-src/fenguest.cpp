#include "fenguest.h"

FenGuest::FenGuest() : QWidget()
{

    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()),this, SLOT(dataRec()));
    connect(socket, SIGNAL(connected()),this, SLOT(guestCo()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(guestDisc()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(erreurSocket(QAbstractSocket::SocketError)));

    tailleMessage = 0;

    // Ecriture de la fenêtre

    setWindowTitle(tr("Alka-Loqui --- Client"));
    setGeometry(0, 0, 628, 480);

}


void FenGuest::enterPressed()
{
    on_boutSent_clicked();
}

void FenGuest::dataRec()
{
    QDataStream in(socket);

    if (tailleMessage == 0)
    {
        if(socket->bytesAvailable() < (int)sizeof(quint16))
            return;

        in >> tailleMessage;
    }

    if (socket->bytesAvailable() < tailleMessage)
        return;

    QString textRe;
    in >> textRe;

    listeMessages->append(textRe);

    tailleMessage =0;
}

void FenGuest::guestCo()
{
    sentMessage(tr("/pseudo ")+pseudo->text());                                                         //modif
    listeMessages->append(tr("<i>Connexion réussie : <br/> Bienvenue chez Alka-Loqui !</i>"));
    boutCo->setEnabled(true);
}

void FenGuest::guestDisc()
{
    listeMessages->append(tr("<i>A plus tard chez AL</i>"));
}

void FenGuest::erreurSocket(QAbstractSocket::SocketError erreur)
{
    switch(erreur) // On affiche un message différent selon l'erreur qu'on nous indique
    {
        case QAbstractSocket::HostNotFoundError:
            listeMessages->append(tr("<em>ERREUR : le serveur n'a pas pu être trouvé. Vérifiez l'IP et le port.</em>"));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            listeMessages->append(tr("<em>ERREUR : le serveur a refusé la connexion. Vérifiez si le programme \"serveur\" a bien été lancé. Vérifiez aussi l'IP et le port.</em>"));
            break;
        case QAbstractSocket::RemoteHostClosedError:
            listeMessages->append(tr("<em>ERREUR : le serveur a coupé la connexion.</em>"));
            break;
        default:
            listeMessages->append(tr("<em>ERREUR : ") + socket->errorString() + tr("</em>"));
    }
    boutCo->setEnabled(true);

}

void FenGuest::on_boutCo_clicked()
{
    listeMessages->append(tr("<i>Connexion en cours.... Veuillez Patienter.....</i>"));
    boutCo->setEnabled(false);

    socket->abort();
    socket->connectToHost(adrIP->text(), numPort->value());
}

void FenGuest::on_boutSent_clicked()
{
    QString textYHTS = textSe->text();
    sentMessage(textYHTS);                                                                  //modif
}
void FenGuest::sentMessage(QString textYHTS)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);                                          //modif

    out << (quint16) 0;
    out << textYHTS;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    socket->write(paquet);
    textSe->clear();
    textSe->setFocus();
}

void FenGuest::enterPressedCo()
{
    on_boutCo_clicked();
}
