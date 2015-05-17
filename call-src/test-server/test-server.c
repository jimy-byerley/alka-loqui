#include <stdio.h>
#include "../call-server.h"
#include "../utility.h"

int main()
{
	char text[40];
	network_init();
	server_data * server;
	server = start_server_thread(7000);
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
			add_host(server, c_sockaddr(text, 7000));
			printf("+-- added %s\n", text);
		}
	}
	network_end();
	return -1;
}
