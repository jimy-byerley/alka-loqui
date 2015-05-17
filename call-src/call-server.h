#ifndef CALL_SERVER_H
#define CALL_SERVER_H

#include "utility.h"
#include "common.h"

/**
   Attention : Pour windows, qui necessite des opérations avant et apres avoir utilisé l'interface réseau, il ne fait pas oublier d'appeler :
   network_init()    avant tout usage
   network_end()     a la fin du programme
**/

#define HOSTNUMBER_MAX 30

/*
  Structure de données du serveur
  Tous les parametres individuels d'un thread serveur s'y trouvent.
  Doit etre alloué dans la memoire a la création du thread et passé au thread par pointeur.
*/
typedef struct {
	float * soundbuffer;                    // buffer des sons cumulés de tous les clients.
	float * hostsbuffers [HOSTNUMBER_MAX];  // buffers des sons émis par tous les clients.
	unsigned char clocks [HOSTNUMBER_MAX];  // dates des dernières émissions des clients.
	unsigned char alive;                    // drapeau 'doit fonctionner' pour le serveur, mettre à 0 pour qu'il s'arrete.
	unsigned int packetinterval;            // interval de temps (ms) entre chaque reception de packet.
	unsigned int sendinterval;              // interval de temps (ms) entre chaque emission aux clients.
	
	SOCKET socket;              // socket.
	pthread_t thread;           // thread courant.
	
	unsigned int hostnumber;             // nombre d'hotes définis (a prendre en compte: si une machine envoie des données mais n'est pas repertoriée, elle sera ignorée.
	SOCKADDR_IN * hosts[HOSTNUMBER_MAX]; // addresses IP des hotes répertoriés (0.0.0.0 -> pas d'hote).
	float volumes [HOSTNUMBER_MAX];      // volumes d'amplification des sons émis par les clients (utile pour le mute).
	
	sound_packet * transit;              // structure packet de transit : buffer données reçues et envoyée.
	unsigned long transit_size;
	SOCKADDR_IN * source;                // addresse de l'emetteur du packet en transit
	unsigned int source_size;
} server_data;


/*
  thread du serveur: reception et emission des données.
*/
void * sound_server_main(void * params);


#define SPEAK_TIMEOUT 0.5  // secondes

/*
  Retourne 1 si l'hote d'indice hostindex est en train de parler (a emis du son récement)
  0 sinon.
  datas est un pointeur vers la structure de données du serveur.
*/
inline char is_speaking(server_data * datas, int hostindex);

/*
  Déclare un hote dont il faut receptionner les données.
  On lui passe l'addresse de l'hote par 4 octets et l'identifiant unique qui le designera par le reseau.
  L'identifiant ne doit pas etre donné aux autres clients pour qu'ils ne puissent pas falsifier les origines des emsissions.
  datas est un pointeur vers la structure de données du serveur.
  Retourne l'indice de l'hote.
*/
char add_host(server_data * server, SOCKADDR_IN * host);

/*
  enleve un hote de la liste des hotes, est utile pour liberer de la place, quand l'hote s'est déconnecté.
*/
void del_host(server_data * server, const int hostindex);

/*
  Retourne l'indice d'un hote désigné par son ip (4 octets), dans la structure de données du serveur.
*/
int get_hostindex(const server_data * data, const SOCKADDR_IN * host);

/*
  Lance le serveur dans un thread_séparé.
  Le serveur ouvrira le port spécifié.
  Retourne la structure de données du serveur ou 0 (NULL).
*/
server_data * start_server_thread(int port);

/*
  Arrete le serveur designé par son bloc de données. la fonction attend que le thread se termine, ce qui est asser rapide
*/
void stop_server(server_data * data);

#endif