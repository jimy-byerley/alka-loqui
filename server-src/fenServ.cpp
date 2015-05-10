#include "fenserv.h"

QTextEdit *textSer;

FenServ::FenServ()
{
    //Disposition des boutons de la fenêtre
    servStat = new QLabel;
    quitBu = new QPushButton(tr("Quitter"));
    connect(quitBu, SIGNAL(clicked()), qApp, SLOT(quit()));

    textSer = new QTextEdit;
    ligneCom = new QLineEdit;
    quitBu = new QPushButton(tr("Quitter"));
    buSent = new QPushButton(tr("Envoyer"));
    textSer->setReadOnly(true);

    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *layout2 = new QHBoxLayout;
    layout->addWidget(servStat);
    layout->addWidget(textSer);
    layout->addLayout(layout2);
    layout2->addWidget(ligneCom);
    layout2->addWidget(buSent);
    layout2->addWidget(quitBu);
    setLayout(layout);

    connect(quitBu, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(buSent, SIGNAL(clicked()), this, SLOT(enterPressed()));

    setWindowTitle(tr("Alka-Loqui//SERVEUR"));
}

void FenServ::setServStat(QString a)
{
    servStat->setText(a);
}
void FenServ::enterPressed()
{
    if(execCommandServ(ligneCom->text()))
    {
    ligneCom->clear();
    ligneCom->setFocus();
    }
}
void FenServ::on_buSent_clicked()
{
    enterPressed();
}

