#ifndef FENSERV_H
#define FENSERV_H

#include "commande.h"
#include <QtWidgets>
#include <QtNetwork>


class FenServ : public QWidget
{
    Q_OBJECT

public:
    FenServ();
    void setServStat(QString a);
private slots:
    void enterPressed();
    void on_buSent_clicked();

private:
    QLabel *servStat;
    QPushButton *quitBu;
    QPushButton *buSent;
    QLineEdit *ligneCom;
};
extern QTextEdit *textSer;

#endif // FENSERV_H
