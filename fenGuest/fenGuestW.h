#ifndef FENGUESTW_H
#define FENGUESTW_H

#include <QtWidgets>
#include <QtNetwork>
#include <fenguest.h>

class fenGuestW : public QMainWindow
{
    Q_OBJECT
public:
    fenGuestW();


private:
    FenGuest *cerveau;
    QWidget *fenCo;
    QMenu *mnFich;
    QMenu *mnAide;
    QMdiArea *zoneCentr;
    QAction *actConn;
    QAction *actQuit;
    QAction *actHelp;
    QTcpSocket *socket;
    //Element de la fenetre;

    QPushButton *boutSent;
    QTextEdit *listeMessages;
    QLineEdit *textSe;
    QVBoxLayout *layV;
    QHBoxLayout *layB;
    QVBoxLayout *layV2;
    QHBoxLayout *layB2;
    QWidget *mdi;

    //Element de la fenetre de connexion
    QGridLayout *lB;
    QLineEdit *pseudo;
    QLineEdit *adrIP;
    QSpinBox *numPort;
    QPushButton *boutCo;
    QLabel *lab1;
    QLabel *lab2;
    QLabel *lab3;


signals:

public slots:
};


#endif // FENGUESTW_H
