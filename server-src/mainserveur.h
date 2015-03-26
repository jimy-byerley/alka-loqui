#ifndef MAINSERVEUR_H
#define MAINSERVEUR_H

#include <QtWidgets>
#include <QtNetwork>
#include <ctime>

#include "client.h"
#include "commande.h"

#include <QtWidgets>
#include <QtNetwork>

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
};
void sentAll(const QString &text);
void sentOne(const QString &text,Host user);




QList<QTcpSocket *> guests;




#endif // MAINSERVEUR_H
