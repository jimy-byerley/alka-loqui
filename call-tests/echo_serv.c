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

/* uniquement pour windows, mais sera présent dans la boucle principale : init et end */
static void init(void)
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

static void end(void)
{
#ifdef WIN32
    WSACleanup();
#endif
}


typedef struct {
	float volume;
	void * soundbuffer;
} stream_opts;


int stream_callback( const void *input, void *output,
	unsigned long frameCount,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void * userData)
{
	float *in = (float*)input;
	unsigned int i;
	stream_opts * params = (stream_opts*)userData;
	for (i=0; i<frameCount; i++)
	{
		params->soundbuffer[i] = in[i] * params->volume;
	}
	return 0;
}


int main(int argc, const char * argv[])
{
	char c;
	PaStream * stream;
	PaError err;
	stream_opts parameters;
	parameters.volume = 1;
	parameters.soundbuffer = malloc(256 * sizeof(float));
	struct hostent * hostinfo = 0;
	
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock == INVALID_SOCKET)
	{
		perror("socket()");
		exit(errno);
	}
	
	/* init pa */
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
	
	/* end */
	err = Pa_CloseStream(stream);
	closesocket(sock);
	if(err != paNoError) goto error;
	Pa_Terminate();
	free(parameters.soundbuffer);
	return 0;

	error:
		printf( "PortAudio error: %s\n", Pa_GetErrorText(err));
		return -1;
}