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
	char * recv_buffer;
	long recv_buffer_size;
} client_data;


/*
  thread du client: envoie et recoit les données audio.
*/
int sound_client_main(void * params)
{
	clock_t t1, t2, towait;
	int i, n;
	
	client_data * client = (client_data*) params;
	set_nonblocking(client->socket);
	while (client->alive > 0)
	{
		t1 = clock();
		n = recvfrom(client->socket, client->recv_buffer, client->recv_buffer_size, 0, 0, 0);
		if (n > 0)
		{
			ident = *((long*) client->recv_buffer;
			memcpy(client->recv_buffer, client->playbuffer, client->sound_size);
		}
		n = sendto(client->socket, client->recv_buffer, client->recv_buffer_size, 0, client->source, client->source_size);
		t2 = clock();
		towait = client->packetinterval - (t2-t1);
		if (towait > 0) sleep_ms(towait);
	}
	
	pthread_exit(0);
}
