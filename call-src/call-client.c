#include "utility.h"
#include "common.h"

typedef struct {
	float * recordbuffer;
	float * playbuffer;
	
	int packetinterval;
	int sound_size;
	char alive;
	
	SOCKADDR * source;
	int source_size;
	SOCKET socket;
	pthread_t thread;
	
	long hostident;
	sound_packet * transit;
	unsigned long transit_size;
} client_data;


/*
  thread du client: envoie et recoit les données audio.
*/
int sound_client_main(void * params)
{
	clock_t t1, t2, towait;
	int i, n;
	client_data * client = (client_data*) params;
	sound_packet * p = client->transit;
	unsigned long psize = client->transit_size;
	set_blocking(client->socket);
	while (client->alive > 0)
	{
		t1 = clock();
		n = recvfrom(client->socket, client->transit, client->transit_size, 0, 0, 0);
		if (n > 0)
		{
			packet2sound(client->transit, client->playbuffer);
		}
		client->transit->ident = client->hostident;
		memcpy(client->recordbuffer, client->recv_buffer, client->sound_size);
		n = sendto(client->socket, client->transit, client->transit_size, 0, client->source, client->source_size);
		t2 = clock();
		towait = client->packetinterval - (t2-t1);
		if (towait > 0) sleep_ms(towait);
	}
	
	pthread_exit(0);
}

client_data * start_client_thread(char addr[4], int port)
{
	client_data * client;
	stream_opts * stream;
	SOCKET sock = 0;
	SOCKADDR_IN destination = {0};
	pthread_t client_thread;
	
	/** open socket **/
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		perror("socket()");
		return 0;
	}
	destination.sin_addr  = {addr[0], addr[1], addr[2], addr[4]};
	destination.sin_port  = htons(port);
	destination.sin_famiy = AF_INET;
	
	/** start Portaudio and stream callback **/
	stream = malloc(sizeof(stream_opts));
	stream->recordbuffer  = malloc(sizeof(float)*SOUND_SIZE);
	stream->playbuffer    = malloc(sizeof(float)*SOUND_SIZE);
	stream->record_volume = 1.0;
	stream->play_volume   = 1.0;
	
	err = Pa_Initialize();
	if (err != paNoError) goto pa_error;
	err = Pa_OpenDefaultStream( 
		&stream,
		1,               /* input channel */
		0,               /* stero input */
		paFloat32,       /* 32 bit float */
		7100,            /* taux d'echantillonnage : 14400 */
		SOUND_SIZE,      /* echantillons par buffer, utiliser paFramesPerBufferUnspecified pour laisser pa choisir le meilleur */
		stream_callback, /* callback de traitenement des données */
		stream           /* pointeur vers les données passées au callback */
	);
	if (err != paNoError) goto pa_error;
	/* maintenant le callback va etre utilisé */
	err = Pa_StartStream(stream);
	if (err != paNoError) goto pa_error;
	
	/** launch thread **/
	
	return client_data;
	
	pa_error:
		printf( "PortAudio error: %s\n", Pa_GetErrorText(err));
		closesoket(sock);
		return 0;
}