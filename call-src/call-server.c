#include "call-server.h"

/**
   Attention : Pour windows, qui necessite des opérations avant et apres avoir utilisé l'interface réseau, il ne fait pas oublier d'appeler :
   network_init()    avant tout usage
   network_end()     a la fin du programme
**/

/*
  thread du serveur: reception et emission des données.
*/
void * sound_server_main(void * params)
{
	clock_t t1, t2, towait, t3, t4 = 0;
	int n, i, j;
	
	server_data * server = (server_data*) params;
	sound_packet * p = server->transit;
	unsigned long psize = server->transit_size;
	
	set_nonblocking(server->socket);   // si aucun packet n'est arrivé, le programme continue sans attendre qu'il n'en arrive.
	while (server->alive > 0)
	{
		t1 = clock();
		n = recvfrom(server->socket, p, psize, 0, 0, 0);
		if (n > 0 && p->ident != 0) // ident=0  =>  client offline
		{
			for (i=0; i<HOSTNUMBER_MAX; i++)
				if (server->idents[i] == p->ident)
				{
					// add sound on global layer (soundbuffer)
					for (j=0; j<SOUND_SIZE; j++)
						server->soundbuffer[j] += ((float*)(p->sound))[j] - server->hostsbuffers[i][j];
					// copy host's sound on host's layer
					packet2sound(p, server->hostsbuffers[p->ident]);
					server->clocks[i] = t1;
					break;
				}
			t4 = clock();
			if (t4-t3 > server->sendinterval)
				for (i=0; i<HOSTNUMBER_MAX; i++)
					if (server->idents[i] != 0)
					{
						p->ident = server->idents[i];
						// copy global layer in packet
						sound2packet(server->soundbuffer, p);
						// remove from packet the host's layer
						for (j=0; j<SOUND_SIZE; j++)
							((float*)p->sound)[j] -= server->hostsbuffers[i][j];
						n = sendto(server->socket, p, psize, 0, server->source, server->source_size);
					}
			t2 = clock();
			towait = server->packetinterval - (t2-t1)/CLOCKS_PER_SEC * 1000; // esperons que t2-t1 soit > 0
			if (towait > 0) sleep_ms(towait);  // la boucle ne doit pas se répeter trop souvent (ne pas bouffer l'UC).
		}
	}
	
	pthread_exit(0);
}


#define SPEAK_TIMEOUT 0.5  // secondes

/*
  Retourne 1 si l'hote d'indice hostindex est en train de parler (a emis du son récement)
  0 sinon.
  datas est un pointeur vers la structure de données du serveur.
*/
inline char is_speaking(server_data * datas, int hostindex)
{
	if (hostindex == -1 || hostindex > HOSTNUMBER_MAX) return 1;
	if (datas->clocks[hostindex] < clock() - SPEAK_TIMEOUT*CLOCKS_PER_SEC)
		return 1;
	else return 0;
}

/*
  Déclare un hote dont il faut receptionner les données.
  On lui passe l'addresse de l'hote par 4 octets et l'identifiant unique qui le designera par le reseau.
  L'identifiant ne doit pas etre donné aux autres clients pour qu'ils ne puissent pas falsifier les origines des emsissions.
  datas est un pointeur vers la structure de données du serveur.
  Retourne l'indice de l'hote.
*/
char add_host(server_data * server, char host[4], long ident)
{
	unsigned int number;
	for (number=0; number<HOSTNUMBER_MAX; number++) if ((int)(server->hosts[number]) == 0) break;
	if (number > HOSTNUMBER_MAX) return -1;
	server->hosts[number][0] = host[0];
	server->hosts[number][1] = host[1];
	server->hosts[number][2] = host[2];
	server->hosts[number][3] = host[3];
	server->idents[number] = ident;
	server->volumes[number] = 1.0;
	if (server->hostsbuffers[number] == 0)  server->hostsbuffers[number] = malloc(SOUND_BUFFER_SIZE * sizeof(float));
	server->hostnumber ++;
	return number;
}

/*
  enleve un hote de la liste des hotes, est utile pour liberer de la place, quand l'hote s'est déconnecté.
*/
void del_host(server_data * server, int hostindex)
{
	free(server->hostsbuffers);
	server->hosts[hostindex][0] = 0;
	server->hosts[hostindex][1] = 0;
	server->hosts[hostindex][2] = 0;
	server->hosts[hostindex][3] = 0;
	server->idents[hostindex] = 0;
	server->hostnumber --;
}

/*
  Retourne l'indice d'un hote désigné par son ip (4 octets), dans la structure de données du serveur.
*/
int get_hostindex(server_data * data, char host[4])
{
	int i = 0;
	for (i; i<HOSTNUMBER_MAX; i++)
		if ((int)(data->hosts[i]) == (int)host)
			return i;
	return -1;
}

/*
  Lance le serveur dans un thread_séparé.
  Le serveur ouvrira le port spécifié.
  Retourne la structure de données du serveur ou 0 (NULL).
*/
server_data * start_server_thread(int port)
{
	server_data * server;
	SOCKET sock = 0;
	SOCKADDR_IN source    = {0};
	pthread_t server_thread;
	
	sock = socket(AF_INET, SOCK_DGRAM, 0); // crée un socket UDP
	if (sock == INVALID_SOCKET)
	{
		perror("socket()");
		return 0;
	}
	source.sin_addr.s_addr = htons(INADDR_ANY);
	source.sin_family     = AF_INET;
	source.sin_port       = htons(port);
	if (bind(sock, (SOCKADDR*) &source, sizeof(source)) == SOCKET_ERROR)
	{
		perror("bind()");
		return 0;
	}
	server = malloc(sizeof(server_data));
	server->transit      = malloc(sizeof(sound_packet));
	server->transit_size = sizeof(sound_packet);
	server->hostnumber   = 0;
	server->alive        = 1;
	server->socket       = sock;
	server->source       = (SOCKADDR*) &source;
	server->source_size  = sizeof(source);
	server->packetinterval  = 50;
	server_thread = pthread_create(&server_thread, 0, sound_server_main, &server);
	server->thread = server_thread;
	
	return server;
}

void stop_server(server_data * data)
{
	unsigned int i;
	data->alive = 0;
	pthread_join(data->thread, 0);
	closesocket(data->socket);
	for (i=0; i<data->hostnumber; i++)
		free(data->hostsbuffers[i]);
	free(data->soundbuffer);
	free(data);
}
