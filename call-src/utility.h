#ifndef UTILITY_H
#define UTILITY_H


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "errno.h"
#include <pthread.h>

#include <fcntl.h>


/* netowrk sockets */
#ifdef WIN32 /* si vous êtes sous Windows */
	#include <winsock2.h>

#elif defined (linux) /* si vous êtes sous Linux */
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h> /* close */
	#include <netdb.h> /* gethostbyname */
	#define INVALID_SOCKET -1
	#define SOCKET_ERROR -1
	#define closesocket(s) close(s)
	
	typedef int SOCKET;
	typedef struct sockaddr_in SOCKADDR_IN;
	typedef struct sockaddr SOCKADDR;
	typedef struct in_addr IN_ADDR;

#else /* sinon vous êtes sur une plateforme non supportée */
	#error not defined for this platform
#endif

/* time sleeping */
#ifdef WIN32
	#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
	#include <time.h>   // for nanosleep
#else
	#include <unistd.h> // for usleep
#endif


extern void sleep_ms(int milliseconds); // cross-platform sleep function

/* uniquement pour windows, mais sera présent dans la boucle principale : init et end */
extern void network_init(void);
extern void network_end(void);

/* methode simple pour parametrer qu'un socket devraa attendre le prochain packet si il n'en est pas en attente */
extern int set_nonblocking(const int descriptor);
extern int set_blocking(const int descriptor);

/*
  Configure une addresse de socket C a partir d'une IP (v4 ou v*6) en str et d'un uméro de port.
  L'adresse retournée est a liberée après usage (free())
  Si il est impossible de créer l'addresse (mauvais format d'addresse), 0 est retourné.
*/
SOCKADDR_IN * c_sockaddr(const char * straddr, const int port);

#endif