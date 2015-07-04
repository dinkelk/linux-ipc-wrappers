#include "SocketWrapper.h"

int main()
{
	char google[] = "173.194.79.103";
	char message[] = "GET / HTTP/1.1\r\n\r\n";
	char reply[2000];
	int port = 80;
	int ret, sd;
	
	sd = socketCreate();
	printf("Created socket wrapper with descriptor %d.\n", sd);

	ret = socketConnect(sd, google, port);
	printf("Socket wrapper connected to '%s' on port '%d' with return value %d.\n", google, port, ret);

	ret = socketSend(sd, message, -1);
	printf("Socket wrapper sent: \n\n'%s'\n\nto '%s' on port '%d' with return value %d.\n", message, google, port, ret);

	ret = socketReceive(sd, reply, 2000);
	printf("Socket wrapper received: \n\n'%s'\n\nfrom '%s' on port '%d' with return value %d.\n", reply, google, port, ret);

	ret = socketDisconnect(sd);
	printf("Socket wrapper disconnected from '%s' on port '%d' with return value %d.\n", google, port, ret);
	
	return 0;
}
