#ifndef FENGUEST_H
#define FENGUEST_H

#include <QtWidgets>
#include <QtNetwork>

class FenGuest : public QWidget
{
public:
    FenGuest();

private slots:
    void enterPressed();
    void enterPressedCo();
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
    QPushButton *boutSent;
    QLineEdit *adrIP;
    QSpinBox *numPort;
    QTextEdit *listeMessages;
    QLineEdit *pseudo;
    QLineEdit *textSe;
    QVBoxLayout *layV;
    QHBoxLayout *layB;
    QVBoxLayout *layV2;
    QHBoxLayout *layB2;

};

#endif // FENGUEST_H
