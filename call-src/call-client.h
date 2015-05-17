#ifndef CALL_CLIENT_H
#define CALL_CLIENT_H

#include "utility.h"
#include "common.h"
#include "portaudio.h"

/**
   Attention : Pour windows, qui necessite des opérations avant et apres avoir utilisé l'interface réseau, il ne fait pas oublier d'appeler :
   network_init()    avant tout usage
   network_end()     a la fin du programme
**/

typedef struct {
	float * recordbuffer;       // buffer du son enregistré par le client (a transmettre au serveur).
	float * playbuffer;         // buffer du son a lire par le client (reçu du serveur).
	
	int packetinterval;         // interval de temps (ms) minimal entre chaque reception et emission réseau.
	char alive;                 // drapeau 'doit fonctionner' pour le client, à mettre à 0 pour arreter le client.
	
	SOCKADDR * destination;     // addresse du serveur.
	int destination_size;       // pour l'optimisation: sizeof(destination)
	SOCKET socket;              // socket.
	pthread_t thread;           // thread du client.
	PaStream * stream;          // structure du flux capture portaudio.
	stream_opts * stream_options;  // structure de parametres pour le callback de son.
	
	sound_packet * transit;     // structure packet de transit : buffer données reçues et envoyée.
	unsigned long transit_size;
} client_data;


/*
  thread du client: envoie et recoit les données audio.
*/
void * sound_client_main(void * params);

/*
 L ance le thread client en utisant l'addresse serveur donnée.                *
 Retourne 1 si le client a été lancé avec succès, 0 sinon.
 */
client_data * start_client_thread(SOCKADDR_IN * address);

/*
  Arrete le client déja lancé. retourne 1 si l'arret est réussi, 0 sinon.
*/
char stop_client(client_data * data);

#endif
