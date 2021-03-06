#include "call-client.h"
#include "stdio.h"

/**
   Attention : Pour windows, qui necessite des opérations avant et apres avoir utilisé l'interface réseau, il ne fait pas oublier d'appeler :
   network_init()    avant tout usage
   network_end()     a la fin du programme
**/

/*
  thread du client: envoie et recoit les données audio.
*/
void * sound_client_main(void * params)
{
	clock_t t1, t2, towait;
	int n;
	client_data * client = (client_data*) params;
	sound_packet * p = client->transit;
	unsigned long psize = client->transit_size;
	set_blocking(client->socket);
	
	while (client->alive > 0)
	{
		t1 = clock();
		n = recv(client->socket, client->transit, client->transit_size, 0);
		if (n > 0)
		{
			printf("%s", (char*)p->sound);
			fflush(stdout);
			packet2sound(p, client->playbuffer);
		}
		sound2packet(client->recordbuffer, p);
		n = sendto(client->socket, p, psize, 0, client->destination, client->destination_size);
		if (n == -1) perror("sound_client_main(): sendto()");
		t2 = clock();
		towait = client->packetinterval - ((float)(t2-t1))/CLOCKS_PER_SEC * 1000;
		if (towait > 0) sleep_ms(towait);
	}
	
	pthread_exit(0);
	return 0;
}

/*
  Lance le thread client en utisant l'addresse serveur donnée.
  Retourne 1 si le client a été lancé avec succès, 0 sinon.
*/
client_data * start_client_thread(SOCKADDR_IN * address)
{
	client_data * client;
	stream_opts * soundopts;
	SOCKET sock = 0;
	PaStream * stream;
	PaError err;
	pthread_t client_thread;
	
	/** open socket **/
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		perror("socket()");
		return 0;
	}
	
	/** start Portaudio and stream callback **/
	soundopts = malloc(sizeof(stream_opts));
	soundopts->recordbuffer  = malloc(sizeof(float)*SOUND_SIZE);
	soundopts->playbuffer    = malloc(sizeof(float)*SOUND_SIZE);
	soundopts->record_volume = 1.0;
	soundopts->play_volume   = 1.0;
	
	err = Pa_Initialize();
	if (err != paNoError) goto pa_error;
	err = Pa_OpenDefaultStream( 
		&stream,
		1,               /* input channel */
		1,               /* output channel */
		paFloat32,       /* 32 bit float */
		7100,            /* taux d'echantillonnage : 14400 */
		SOUND_SIZE,      /* echantillons par buffer, utiliser paFramesPerBufferUnspecified pour laisser pa choisir le meilleur */
		stream_callback, /* callback de traitenement des données */
		soundopts        /* pointeur vers les données passées au callback */
	);
	if (err != paNoError) goto pa_error;
	/* maintenant le callback va etre utilisé */
	err = Pa_StartStream(stream);
	if (err != paNoError) goto pa_error;
	
	/** launch thread **/
	client = malloc(sizeof(client_data));
	client->alive            = 1;
	client->destination      = (SOCKADDR*) address;
	client->destination_size = sizeof(SOCKADDR);
	client->socket           = sock;
	client->recordbuffer     = soundopts->recordbuffer;
	client->playbuffer       = soundopts->playbuffer;
	client->transit          = malloc(sizeof(sound_packet));
	client->transit_size     = sizeof(sound_packet);
	client->packetinterval   = 10;
	client->stream           = stream;
	client->stream_options   = soundopts;
	client_thread = pthread_create(&client_thread, 0, sound_client_main, client);
	client->thread = client_thread;
	
	return client;
	
	pa_error:
		printf( "PortAudio error: %s\n", Pa_GetErrorText(err));
		closesocket(sock);
		return 0;
}

/*
  Arrete le client déja lancé. retourne 1 si l'arret est réussi, 0 sinon.
*/
char stop_client(client_data * data)
{
	PaError err;
	err = Pa_CloseStream(data->stream);
	if(err != paNoError)
	{
		printf( "PortAudio error: %s\n", Pa_GetErrorText(err));
		closesocket(data->socket);
		return 0;
	}
	Pa_Terminate();
	
	data->alive = 0;
	pthread_join(data->thread, 0);
	closesocket(data->socket);
	free(data->destination);
	free(data->recordbuffer);
	free(data->playbuffer);
	free(data->transit);
	free(data->stream_options);
	free(data);
	return 1;
}
