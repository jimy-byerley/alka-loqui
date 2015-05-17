#include "../call-client.h"
#include "stdio.h"

int main()
{
	client_data * client;
	network_init();

	client = start_client_thread(c_sockaddr("127.0.0.1", 7000));
	printf("press enter to quit.\n");
	getchar();
	stop_client(client);
	
	network_end();
	return 0;
}
