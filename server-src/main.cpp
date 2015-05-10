#include "fenserv.h"
#include "mainserveur.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FenServ fen;
    fen.show();
    mainserveur b;
    fen.setServStat(b.demarage());
    return a.exec();
}
