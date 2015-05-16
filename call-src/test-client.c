#include "call-client.h"
#include "stdio.h"

int main(int argc, char ** argv)
{
	client_data * client;
	unsigned long ident = 0;
	
	if (argc > 1)
	{
		sscanf(argv[1], "%ld", &ident);
	}
	if (argc <= 1 || ident == 0)
	{
		fprintf(stderr, "error: 1 arg should have been passed (different to 0).\n");
		return -1;
	}
	client = start_client_thread("127.0.0.1", 7000, ident);
	printf("press enter to quit.\n");
	getchar();
	stop_client(client);
	
	return -1;
}
