#ifndef MAINSERVEUR_H
#define MAINSERVEUR_H

#include <QtWidgets>
#include <QtNetwork>
#include <ctime>

#include "commande.h"
#include "client.h"

class mainserveur : public QWidget
{
        Q_OBJECT
public:
    mainserveur();
    QString demarage();
private slots:
    void newCon();
    void dataRec();
    void discGuest();
private:
    quint16 tailleMessage;
    QTcpServer *server;
    QList<QTcpSocket *> guests;
    QList<Host> cGuest;
};

void sentAll(const QString &text, QList<Host> cGuest);
void sentOne(const QString &text, Host user);









#endif // MAINSERVEUR_H
