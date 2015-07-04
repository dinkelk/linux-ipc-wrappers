#include "Message.h"

void clearMessage(Message* msg)
{
	memset(msg->buffer, 0, sizeof(msg->buffer));
	msg->err = 0;
	msg->mtype = 0;
}

void setMessage(Message* msg, char* buffer, int len, int mtype)
{	
	assert(len <= MESSAGE_SIZE);
	memcpy(msg->buffer, buffer, len);
	msg->err = 0;
	msg->mtype = mtype;
}

const char* messageToString(Message* msg)
{
	static char toReturn[255];
	memset(toReturn, 0, 255);

	sprintf(toReturn, "mtype: %ld\nerr: %d\nlen: %u\nbuffer: %s\n",
			msg->mtype,
			msg->err,
			sizeof(Message),
			msg->buffer);

	return toReturn;
}
