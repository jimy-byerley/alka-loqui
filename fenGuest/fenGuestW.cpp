#include "fenGuestW.h"
#include "fenguest.h"

fenGuestW::fenGuestW() : QMainWindow()
{

    // Création des élements
    fenCo = new QWidget;
    zoneCentr = new QMdiArea;
    mnFich = new QMenu;
    mnAide = new QMenu;
    actConn = new QAction("&Connection", this);
    actQuit = new QAction("&Quitter", this);
    actHelp = new QAction("Qui Sommes Nous ?..", this);
    listeMessages = new QTextEdit;
    boutSent = new QPushButton(tr("&Envoyer"));
    textSe = new QLineEdit;
    layV = new QVBoxLayout;
    layB = new QHBoxLayout;
    QLabel *label = new QLabel(tr("Message : "));
    mdi = new QWidget;

    //Ecritre de la fenêtre de connexion

    fenCo->setWindowTitle(tr("Connexion."));
    lB = new QGridLayout;
    pseudo = new QLineEdit;
    adrIP = new QLineEdit;
    boutCo = new QPushButton(tr("Connexion"));
    numPort = new QSpinBox;
    lab1 = new QLabel(tr("Ip du Serveur : "));
    lab2 = new QLabel(tr("Port du serveur : "));
    lab3 = new QLabel(tr("Pseudo : "));


    fenCo->setLayout(lB);
    lB->addWidget(lab3, 0, 0);
    lB->addWidget(pseudo, 0, 1);
    lB->addWidget(lab1, 1, 0);
    lB->addWidget(adrIP,1,1);
    lB->addWidget(lab2,2,0);
    lB->addWidget(numPort,2,1);
    lB->addWidget(boutCo,3,0,1,2);

                    //definition des valeurs par défaut de widget de la feneêtre de connection

    numPort->setMaximum(65535);
    numPort->setValue(42424);
    adrIP->setText("127.0.0.1");

    // Ecriture de la fenêtre

    setWindowTitle(tr("Alka-Loqui --- Client"));
    setGeometry(0, 0, 628, 480);
    setCentralWidget(zoneCentr);
    zoneCentr->setLayout(layV);
    layV->addWidget(listeMessages);
    layV->addLayout(layB);
    layB->addWidget(label);
    layB->addWidget(textSe);
    layB->addWidget(boutSent);


    mnFich = menuBar()->addMenu(tr("&Fichier")); //Création de l'onglet Fichier, le & devant permet d'utiliser le raccourci CTRL+F pour ouvrir le menu
    mnFich->addAction(actConn);
    mnFich->addAction(actQuit);
    mnAide = menuBar()->addMenu(tr("&Aide")); //De même pour Aide
    mnAide->addAction(actHelp);

    // definition des valeurs des widgets

    listeMessages->setReadOnly(true);

    //Connexion

    connect(actQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(actConn, SIGNAL(triggered()), fenCo, SLOT(show()));



    //Raccourci

    actQuit->setShortcut(QKeySequence("Ctrl+Q"));
    actConn->setShortcut(QKeySequence("Alt+C"));


}



