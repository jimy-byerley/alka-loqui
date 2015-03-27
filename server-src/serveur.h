#ifndef SERVEUR_H
#define SERVEUR_H

#include <QtNetwork>

void serveur();
void newCon();
void dataRec();
void discGest();
void sentAll(const QString &text);


#endif // SERVEUR_H
