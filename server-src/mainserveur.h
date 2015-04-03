#ifndef MAINSERVEUR_H
#define MAINSERVEUR_H

#include <QtWidgets>
#include <QtNetwork>
#include <ctime>
#include <pthread.h>

#include "commande.h"
#include "client.h"

typedef struct
{
    QTcpSocket *socket;
    quint16 tailleMessage;

}nCo;

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
    int found_nCo(QTcpSocket *a);

private:
    quint16 tailleMessage;
    QTcpServer *server;
    QList<nCo> guests;
};

bool operator==(nCo a,nCo b);
void sentAll(QString text);
void sentOne(QString text, Host user);









#endif // MAINSERVEUR_H
