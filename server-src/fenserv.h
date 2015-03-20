#ifndef FENSERV_H
#define FENSERV_H

#include <QtWidgets>
#include <QtNetwork>
#include "client.h"

class FenServ : public QWidget
{
    Q_OBJECT

public:
    FenServ();
    void sentAll(const QString &text);

private slots:
    void newCon();
    void dataRec();
    void discGuest();

private:

    QLabel *servStat;
    QPushButton *quitBu;
    QTcpServer *server;
    QList<QTcpSocket *> guests;
    quint16 tailleMessage;

};

#endif // FENSERV_H
