#include "fenguest.h"
#include "fenGuestW.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FenGuest fen;
    fen.show();

    return a.exec();
}
