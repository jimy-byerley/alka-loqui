#ifndef FENGUEST_H
#define FENGUEST_H

#include <QtWidgets>
#include <QtNetwork>


class FenGuest : public QMainWindow
{
    Q_OBJECT
public:
    FenGuest();

  public slots:
    void dataRec();
    void guestCo();
    void guestDisc();
    void erreurSocket(QAbstractSocket::SocketError erreur);
    void sentMessage(QString textYHTS);
    void on_boutCo_clicked();

    void on_boutSent_clicked();

private:

     QTcpSocket *socket;
     quint16 tailleMessage;
    //Element de la fenetre;
     QPushButton *boutCo;
     QLineEdit *adrIP;
     QSpinBox *numPort;
     QTextEdit *listeMessages;
     QLineEdit *pseudo;
     QLineEdit *textSe;
     QPushButton *boutSent;
     QWidget *fenCo;
     QWidget *mdi;
     QMdiArea *zoneCentr;
     QMenu *mnFich;
     QMenu *mnAide;
     QAction *actConn;
     QAction *actQuit;
     QAction *actHelp;
     QVBoxLayout *layV;
     QHBoxLayout *layB;
     QGridLayout *lB;
     QLabel *lab1;
     QLabel *lab2;
     QLabel *lab3;

};

#endif // FENGUEST_H
