#include "fenserv.h"

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
}

void FenServ::setServStat(QString a)
{
    servStat->setText(a);
}

