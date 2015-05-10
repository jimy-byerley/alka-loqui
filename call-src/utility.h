#ifndef UTILITY_H
#define UTILITY_H


#include <stdio.h>
#include <math.h>
#include "portaudio.h"
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



void sleep_ms(int milliseconds) // cross-platform sleep function
{
#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = milliseconds * 1000000;
    nanosleep(&ts, NULL);
#else
    usleep(milliseconds * 1000);
#endif
}


/* uniquement pour windows, mais sera présent dans la boucle principale : init et end */
static void network_init(void)
{
#ifdef WIN32
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(err < 0)
    {
        puts("WSAStartup failed !");
        exit(EXIT_FAILURE);
    }
#endif
}

static void network_end(void)
{
#ifdef WIN32
    WSACleanup();
#endif
}


int set_nonblocking(const int descriptor)
{
	#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
	
	int nonBlocking = 1;
	if ( fcntl( descriptor, F_SETFL, O_NONBLOCK, nonBlocking ) == -1 )
	{
		printf( "failed to set non-blocking socket\n" );
		return -1;
	}
	
	#elif PLATFORM == PLATFORM_WINDOWS
	
	DWORD nonBlocking = 1;
	if ( ioctlsocket( descriptor, FIONBIO, &nonBlocking ) != 0 )
	{
		printf( "failed to set non-blocking socket\n" );
		return -1;
	}
	
	#endif
	
	/*
	fcntl(descriptor, F_SETFL, O_NONBLOCK);
	*/
	/*
	if (descriptor < 0) return -1;

	char blocking = 0;
	#ifdef WIN32
		return (ioctlsocket(descriptor, FIONBIO, &blocking) == 0) ? 0 : -1;
	#else
		int flags = fcntl(descriptor, F_GETFL, 0);
		if (flags < 0) return -1;
		flags = blocking ? (flags&~O_NONBLOCK) : (flags|O_NONBLOCK);
		return (fcntl(descriptor, F_SETFL, flags) == 0) ? 0 : -1;
	#endif
	*/
	/*
	// where socket descriptor is the socket you want to make non-blocking
	int status = fcntl(descriptor, F_SETFL, fcntl(descriptor, F_GETFL, 0) | O_NONBLOCK);

	if (status == -1)
		perror("calling fcntl");
	// handle the error.  By the way, I've never seen fcntl fail in this way
	*/
}

int set_blocking(const int descriptor)
{
}


#endif