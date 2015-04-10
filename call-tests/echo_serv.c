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


#define min(a,b) (a<b?a:b)


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


typedef struct {
	float volume;
	float * soundbuffer;
} stream_opts;


int stream_callback( const void *input, void *output,
	unsigned long frameCount,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void * userData)
{
	float *out = (float*)output;
	unsigned int i;
	stream_opts * params = (stream_opts*)userData;
	for (i=0; i<frameCount; i++)
	{
		out[i] = params->soundbuffer[i] * params->volume;
	}
	return 0;
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



typedef struct {
	float * soundbuffer;
	char alive;
	int packetspace;
	SOCKADDR * source;
	int source_size;
	SOCKET socket;
} network_opts;

int recv_datas(void * params)
{
	clock_t t1, t2, towait;
	int n;
	network_opts * netparams = (network_opts*) params;
	static char * buffer[1024];
	
	set_nonblocking(netparams->socket);
	while (netparams->alive > 0)
	{
		t1 = clock();
		n = recvfrom(netparams->socket, buffer, sizeof buffer, 0, 0, 0);
 		//n = recvfrom(*(netparams->socket), buffer, sizeof(buffer), MSG_DONTWAIT, netparams->source, &(netparams->source_size));
		printf("%d\n", n);
		if (n > 0)
		{
			memcpy(netparams->soundbuffer, buffer, 1024);
			t2 = clock();
			towait = netparams->packetspace - (t2-t1)/CLOCKS_PER_SEC * 1000;
			if (towait > 0) sleep_ms(towait);
		}
	}
	
	pthread_exit(0);
}


int main(int argc, const char * argv[])
{
	char c;
	
	PaStream * stream;
	PaError err;
	stream_opts parameters;
	parameters.volume = 1;
	parameters.soundbuffer = malloc(256 * sizeof(float));
	
	SOCKET sock = 0;
	network_opts netparams;
	pthread_t receiver;
	
	
	network_init();
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock == INVALID_SOCKET)
	{
		perror("socket()");
		exit(errno);
	}
	SOCKADDR_IN server     = {0};
	server.sin_addr.s_addr = inet_addr("127.0.0.1"); //htonl(INADDR_ANY);
	server.sin_family      = AF_INET;
	server.sin_port        = htons(7000);
	if (bind(sock, (SOCKADDR*) &server, sizeof(server)) == SOCKET_ERROR)
	{
		perror("bind()");
		exit(errno);
	}
	netparams.soundbuffer = parameters.soundbuffer;
	netparams.alive       = 1;
	netparams.socket      = sock;
	netparams.source      = (SOCKADDR*) &server;
	netparams.source_size = sizeof(server);
	netparams.packetspace = 50;
	
	
	/** init pa **/
	err = Pa_Initialize();
	if (err != paNoError) goto error;
	err = Pa_OpenDefaultStream( 
		&stream,
		0,         /* input channel */
		1,         /* stero input */
		paFloat32, /* 32 bit float */
		7100, /* taux d'echantillonnage : 14400 */
		256,   /* echantillons par buffer, utiliser paFramesPerBufferUnspecified pour laisser pa choisir le meilleur */
		stream_callback,
		&parameters /* pointeur vers les données passées au callback */
	);
	if (err != paNoError) goto error;
	/* maintenant le callback va etre utilisé */
	err = Pa_StartStream(stream);
	if (err != paNoError) goto error;
	
	receiver = pthread_create(&receiver, 0, recv_datas, &netparams);
	
	/** runtime **/
	/* appui sur une touche pour quitter */
	printf("appuyer sur une 'q' pour arreter. (U et I pour modifier le volume)\n");
	do
	{
		c = getchar();
		if (c == 'u') parameters.volume -= 0.1;
		else if (c == 'i') parameters.volume += 0.1;
	} while (c != 'q');
	
	/* arret de pa */
	/*
	Pa_AbortStream(stream);
	if (err != paNoError) goto error;
	*/
	
	/** end PA **/
	err = Pa_CloseStream(stream);
	closesocket(sock);
	if(err != paNoError) goto error;
	Pa_Terminate();
	
	/* fin des threads */
	netparams.alive = 0;
	printf("waiting for the thread to stop.\n");
	pthread_join(receiver, 0);
	pthread_exit(0);
	/* desactivation du la prise en charge des sockets (windows) */
	network_end();
	free(parameters.soundbuffer);
	free(netparams.soundbuffer);
	return 0;

	error:
		printf( "PortAudio error: %s\n", Pa_GetErrorText(err));
		return -1;
}