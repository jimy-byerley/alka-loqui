#ifndef FENSERV_H
#define FENSERV_H

#include <QtWidgets>
#include <QtNetwork>

class FenServ : public QWidget
{
    Q_OBJECT

public:
    FenServ();
    void setServStat(QString a);

private:

    QLabel *servStat;
    QPushButton *quitBu;
};

#endif // FENSERV_H
