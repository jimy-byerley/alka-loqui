#include "fenguest.h"



FenGuest::FenGuest() : QMainWindow()
{

    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()),this, SLOT(dataRec()));
    connect(socket, SIGNAL(connected()),this, SLOT(guestCo()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(guestDisc()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(erreurSocket(QAbstractSocket::SocketError)));

    tailleMessage = 0;

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
    QLabel *labImg = new QLabel;
    labImg->setPixmap(QPixmap("pinguin.png"));
    boutCall = new QPushButton("Appeler");



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
    lB->addWidget(labImg,4,0);

                    //definition des valeurs par défaut de widget de la feneêtre de connection

    numPort->setMaximum(65535);
    numPort->setValue(42424);
    adrIP->setText("127.0.0.1");

    // Ecriture de la fenêtre

    setWindowTitle(tr("Alka-Loqui --- Client"));
    setWindowIcon(QIcon("logo.png"));
    setCentralWidget(zoneCentr);
    zoneCentr->setStyleSheet("background-image : url(logo.png) fixed no-repeat;");
    zoneCentr->setLayout(layV);
    layV->addWidget(listeMessages);
    layV->addLayout(layB);
    layB->addWidget(label);
    layB->addWidget(textSe);
    layB->addWidget(boutSent);
    layB->addWidget(labImg);
    layB->addWidget(boutCall);
    boutCall->setIcon(QIcon("telIcon.jpg"));



    mnFich = menuBar()->addMenu(tr("&Fichier")); //Création de l'onglet Fichier, le & devant permet d'utiliser le raccourci CTRL+F pour ouvrir le menu
    mnFich->addAction(actConn);
    mnFich->addAction(actQuit);
    mnAide = menuBar()->addMenu(tr("&Aide")); //De même pour Aide
    mnAide->addAction(actHelp);

    // definition des valeurs des widgets

    listeMessages->setReadOnly(true);
    listeMessages->setAcceptRichText(true);

    //Connexion

    connect(actQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(actConn, SIGNAL(triggered()), fenCo, SLOT(show()));
    connect(boutCo, SIGNAL(clicked()), this, SLOT(on_boutCo_clicked()));
    connect(boutSent, SIGNAL(clicked()), this, SLOT(on_boutSent_clicked()));



    //Raccourci

    actQuit->setShortcut(QKeySequence("Ctrl+Q"));
    actConn->setShortcut(QKeySequence("Alt+C"));






}
/*void FenGuest::appeler()
{
    client_data * client;
	network_init();

	client = start_client_thread(c_sockaddr("127.0.0.1", 7000));
	printf("press enter to quit.\n");
	getchar();
	stop_client(client);
	
	network_end();
    
}*/


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

void FenGuest::on_boutSent_clicked()
{
    QString textYHTS = textSe->text();
    sentMessage(textYHTS);
}

void FenGuest::on_boutCo_clicked()
{
    listeMessages->append(tr("<i>Connexions en cours... Veuillez patienter....</i>"));
    boutCo->setEnabled(false);
    socket->abort();
    socket->connectToHost(adrIP->text(), numPort->value());
    fenCo->close();
}

