/**********************************************************************
 * File: SocketWrapper.cpp
 * Purpose: Provides TCP/IP communication using sockets.
 * Author: Kevin Dinkel
 **********************************************************************/

/***************************************************
 * Section: Includes
 **************************************************/
#include "SocketWrapper.h"
/***************************************************
 * Function: socketCreate()
 * Purpose: Creates a socket for IPv4, TCP/IP.
 * Returns: The socket_desc on success, -1 on failure.
 **************************************************/
int socketCreate()
{
	int socket_desc;
	int optval = 1;

	/*
	// Options:
	// 1) Address Family: AF_INET (IPv4)
	// 2) Type: SOCK_STREAO (TCP), SOCK_DGRAM (UDP)
	// 3) Protocol: IPPROTO_IP = 0 (IP Protocol)
	*/

	if( (socket_desc = socket(AF_INET , SOCK_STREAM , IPPROTO_IP)) < 0 )
	{
		return errno;
	}	

	/* Make sure we can reuse this port a second time on a crash: */
	setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	return socket_desc;
}

int socketBind(int socket_desc, int port)
{
	struct sockaddr_in server;
	server.sin_addr.s_addr = htonl(INADDR_ANY); /* connect to any ip address associated with me, the server */
	server.sin_family = AF_INET;
    server.sin_port = htons( port ); /* convert port number to standard network format */
 	
	if(bind(socket_desc, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		return errno;
	}

	return 0;
}

int socketListen(int socket_desc, int max_num_clients)
{
	if(listen(socket_desc, max_num_clients) < 0)
	{
		return errno;
	}

	return 0;
}

int socketAccept(int socket_desc)
{
	int connected_socket_desc;
	if( (connected_socket_desc = accept(socket_desc, (struct sockaddr*)NULL, NULL)) < 0 )
	{
		return errno;
	}

	return connected_socket_desc;
}

int socketConnect(int socket_desc, char* ip_address, int port)
{
	struct sockaddr_in server;
	server.sin_addr.s_addr = inet_addr(ip_address); /* convert ip string to long */
	server.sin_family = AF_INET;
    server.sin_port = htons( port ); /* convert port number to standard network format */
 	
 	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        return errno;
    }
 
 	return 0;  
}

int socketDisconnect(int socket_desc)
{
	return close(socket_desc);
}

int socketPoll(int socket_desc, int usecs)
{
	/*
	// Wait for data in receive buffer or until time out before returning
	// Return number of bytes recieved when receive buffer has data
	// Return 0 on a timeoue
	// Return -1 on error
	*/
	int ret;
	struct pollfd pfd;
	struct timespec req;

	pfd.fd = socket_desc;
	pfd.events = POLLIN;

	req.tv_sec = usecs / 1000000;
	req.tv_nsec = (usecs % 1000000) * 1000L;

	if( (ret = ppoll(&pfd, 1, &req, NULL)) <= 0 )
	{
		return ret;
	}

	return ret;
}

int socketSend(int socket_desc, char* message, int length)
{
	if( length < 0 )
		length = strlen(message);

	if( send(socket_desc , message, length, 0) < 0)
    {
        return errno;
    }

    return 0;
}

int socketSend_nowait(int socket_desc, char* message, int length)
{
	if( length < 0 )
		length = strlen(message);

	if( send(socket_desc, message, length, MSG_DONTWAIT) < 0)
    {
        return errno;
    }

    return 0;
}

int socketReceive(int socket_desc, char* server_reply, int length)
{
	int ret;

	if( (ret = recv(socket_desc, server_reply , length , 0)) <= 0) /* len of zero means connection closed */
    {
        return errno;
    }

    return ret;
}

int socketReceive_nowait(int socket_desc, char* server_reply, int length)
{
	int ret;

	if( (ret = recv(socket_desc, server_reply , length , MSG_DONTWAIT)) <= 0) /* len of zero means connection closed */
    {
        return errno;
    }

    return ret;
}

int socketReceive_timeout(int socket_desc, char* server_reply, int length, int usecs)
{
	int ret;

	if( (ret = socketPoll(socket_desc, usecs)) <= 0 )
	{
		return errno;
	}

	return socketReceive_nowait( socket_desc, server_reply, length);
}