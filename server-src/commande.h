#ifndef COMMANDE_H
#define COMMANDE_H

#include<QString>

class Commande
{
    public:
        Commande(const QString &text,int n);
    private slots:
        void ban(QString user);
        void kick(QString user);
};


#endif // COMMANDE_H
