#include "fenserv.h"
#include "commande.h"

FenServ::FenServ()
{
    //Disposition des boutons de la fenêtre
    servStat = new QLabel;
    quitBu = new QPushButton(tr("Quitter"));
    connect(quitBu, SIGNAL(clicked()), qApp, SLOT(quit()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(servStat);
    layout->addWidget(quitBu);
    setLayout(layout);

    setWindowTitle(tr("Alka-Loqui//SERVEUR"));

    //Gérance du serveur

    server = new QTcpServer(this);
    if(!server->listen(QHostAddress::Any, 42424)) //Tout adresse IP et sur port 42424
    {
        servStat->setText(tr("Problème de démarrage serveur dû à <br />") + server->errorString());
    }
    else
    {
        servStat->setText(tr("Démarrage accompli avec succès sur le port <strong>")+ QString::number(server->serverPort()) + tr("</strong>.<br />Les clients peuvent se connecter"));
        connect(server, SIGNAL(newConnection()), this, SLOT(newCon()));
    }

    tailleMessage = 0;

    Host defaut;
    defaut.lvl=-1;
    host<<defaut;
}


