#include "fenserv.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FenServ fen;
    fen.show();

    return a.exec();
}
