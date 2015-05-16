#include <stdio.h>
#include "call-server.h"

int main()
{
	char text[40];
	char addr[7];
	int ident = 1;
	server_data * server;
	server = start_server_thread(30000);
	while (1)
	{
		scanf("%s", text);
		if (strcmp(text, "q") == 0)
		{
			stop_server(server);
			return 0;
		}
		else
		{
			sscanf(text, "%d.%d.%d.%d", addr, addr+1, addr+2, addr+3);
			add_host(server, addr, ident++);
			printf("+-- added %d.%d.%d.%d\n", addr[0], addr[1], addr[2], addr[3]);
		}
	}
	return -1;
}
