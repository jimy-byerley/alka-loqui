#include <stdio.h>
#include "call-server.h"

int main(int argc, char *argv[])
{
	server_data * server;
	server = start_server_thread(30000);
	getchar();
	stop_server(server);
}
