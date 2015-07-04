#define __USE_GNU
#define _GNU_SOURCE
#include "errno.h"
#include <stdio.h>
#include <string.h>     /* strlen */
#include <sys/socket.h> /* sockets */
#include <arpa/inet.h>  /* inet_addr */
#include <unistd.h>     /* write */
#include <sys/poll.h>

/*
 * SocketWrapper Class:
 * This class implements a connection based socket interface implementing
 * TCP/IP assuming IPv4 address types.
 *
 *
 */
int socketCreate();
int socketBind(int socket_desc, int port);
int socketListen(int socket_desc, int max_num_clients);
int socketAccept(int socket_desc);
int socketConnect(int socket_desc, char* ip_address, int port);
int socketDisconnect(int socket_desc);
int socketPoll(int socket_desc, int usecs);
int socketSend(int socket_desc, char* message, int length);
int socketSend_nowait(int socket_desc, char* message, int length);
int socketReceive(int socket_desc, char* server_reply, int length);
int socketReceive_nowait(int socket_desc, char* server_reply, int length);
int socketReceive_timeout(int socket_desc, char* server_reply, int length, int usecs);
int getError();
