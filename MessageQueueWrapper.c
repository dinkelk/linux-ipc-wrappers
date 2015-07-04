/**********************************************************************
 * File: messageQueueWrapper.c
 * Purpose: Provides IPC using message queues.
 * Author: Kevin Dinkel
 **********************************************************************/

/***************************************************
 * Section: Includes
 **************************************************/
#include "MessageQueueWrapper.h"
/***************************************************
 * Function: messageQueueGet(key_t key, int p)
 * Param key: A key (from ftok) used to create a queue.
 * Param p: The priority level of the queue you are creating.
 * Purpose: Creates a queue based on key.
 * Returns: The qid on success, -1 on failure.
 **************************************************/
int messageQueueGet(key_t key)
{
	int qid;
	if ((qid = msgget(key, 0666)) == -1)
		return -1;

	return qid;
}
/***************************************************
 * Function: messageQueueCreate(key_t key, int p)
 * Param key: A key (from ftok) used to create a queue.
 * Param p: The priority level of the queue you are creating.
 * Purpose: Creates a queue based on key.
 * Returns: The qid on success, -1 on failure.
 **************************************************/
int messageQueueCreate(key_t key)
{
	int qid;
	struct msqid_ds buf;

	if ((qid = msgget(key, IPC_CREAT | 0666)) == -1)
		return -1;

	/* increase message queue size */
	if (msgctl(qid, IPC_STAT, &buf) == -1) 
		return -1;

	/* set new buffer size */
	buf.msg_qbytes = 16384; /* look at ipcs -l */
	
	/* set Queue values */
	if (msgctl(qid, IPC_SET, &buf) == -1) 
		return -1;

	return qid;
}
/*********************************************************
 * Function: messageQueueDelete(int qid)
 * Param qid: The queue ID of the message queue to be deleted.
 * Purpose: Deletes a message queue to release system resources.
 * Returns: -1 on failure, 0 on success.
 *********************************************************/
int messageQueueDelete(int qid)
{
	return msgctl(qid, IPC_RMID, NULL);
}
/****************************************************
 * Function: messageQueueSend(int qid, BMsg *msg)
 * Param qid: The queue ID of the message queue.
 * Param msg: A BMsg struct that has been initialized
 * 		  and will be sent over the message queue
 * Purpose: Sends a message over a queue denoted by qid
 * Returns: -1 on failure, 0 on success.
 ****************************************************/
int messageQueueSend(int qid, Message* msg)
{
/*
	struct msqid_ds buf;
	int temp;
	temp = msgctl(qid, IPC_STAT, &buf);
	if (temp == -1) {
		(void) fprintf(stderr, "msgctl fail");
	}
	else {
	   cerr << "msg_perm.uid = "<< buf.msg_perm.uid<< endl;
	   cerr << "msg_perm.gid = "<< buf.msg_perm.gid<< endl;
	   cerr << "msg_perm.cuid = "<< buf.msg_perm.cuid<< endl;
	   cerr << "msg_perm.cgid = "<< buf.msg_perm.cgid<< endl;
	   cerr << "msg_perm.mode = "<< buf.msg_perm.mode;
	   cerr << " access permissions = "<< (buf.msg_perm.mode & 0777)<< endl;
	   cerr << "msg_cbytes = "<< buf.msg_cbytes<< endl;
	   cerr << "msg_qbytes = "<< buf.msg_qbytes<< endl;
	   cerr << "msg_qnum = "<< buf.msg_qnum<< endl;
	   cerr << "msg_lspid = "<< buf.msg_lspid<< endl;
	   cerr << "msg_lrpid = "<< buf.msg_lrpid << endl;
	}
*/
	int ret;
	if ((ret = msgsnd(qid, msg, sizeof(Message) - sizeof(long), 0)) == -1)
	{
		msg->err=errno;
		return -1;
	}
	return ret;
}

int messageQueueSend_nowait(int qid, Message* msg)
{
	int ret;
	if ((ret = msgsnd(qid, msg, sizeof(Message) - sizeof(long), IPC_NOWAIT)) == -1)
	{
		msg->err=errno;
		return -1;
	}
	return ret;
}

int messageQueueSend_timeout(int qid, Message* msg, int usecs)
{
	int ret;
	timer timerid;
	int sleeptime = 500000;
	int st = usecs / 10;

	/* Sleep for half a second before checking timeout, unless it should be smaller. */
	if ( sleeptime > st )
	{
		sleeptime = st; 
	}

	/* Create timeout timer: */
	timerCreate(&timerid);
	timerSet_us(&timerid, usecs);

	/* Reset errno to zero: */
	errno = 0;

	/* Get message: */
	while( (ret = msgsnd(qid, msg, sizeof(Message) - sizeof(long), IPC_NOWAIT)) < 0 )
	{
		if( errno != ENOMSG )
		{
			msg->err=errno;
			timerDelete(&timerid);
			return -1;
		}

		if( isTimerExpired(&timerid) )
		{
			msg->err=errno;
			timerDelete(&timerid);
			return -1; /* return -1 here not 0 because it is a send */
		}

		rt_sleep_us( sleeptime );
	}

	timerDelete(&timerid);

	return ret;
}

/*******************************************************
 * Function: messageQueueReceive(int qid, long type, MMsg *msg)
 * Param qid: The queue ID of the message queue.
 * Param type: The specific type of message you want to get.
 *                 Can be any integer.
 * Param msg: A MMsg struct that will store the return data.
 * Purpose: Receives a message from the message queue.
 * Returns: -1 on failure, 0 on success.
 *********************************************************/
int messageQueueReceive(int qid, Message* msg, long type)
{
	int ret;
	if ((ret = msgrcv(qid, msg, sizeof(Message) - sizeof(long), type, 0)) == -1)
	{
		msg->err=errno;
		return -1;
	}
	return ret;
}

int messageQueueReceive_nowait(int qid, Message* msg, long type)
{
	int ret;
	if ((ret = msgrcv(qid, msg, sizeof(Message) - sizeof(long), type, IPC_NOWAIT)) == -1)
	{
		msg->err=errno;
		return -1;
	}
	return ret;
}

int messageQueueReceive_timeout(int qid, Message* msg, int usecs, long type)
{
	int ret;
	timer timerid;
	int sleeptime = 500000;
	int st = usecs / 10;

	/* Sleep for half a second before checking timeout, unless it should be smaller. */
	if ( sleeptime > st )
	{
		sleeptime = st; 
	}

	/* Create timeout timer: */
	timerCreate(&timerid);
	timerSet_us(&timerid, usecs);

	/* Reset errno to zero: */
	errno = 0;

	/* Get message: */
	while( (ret = msgrcv(qid, msg, sizeof(Message) - sizeof(long), type, IPC_NOWAIT)) < 0 )
	{
		if( errno != ENOMSG )
		{
			msg->err=errno;
			timerDelete(&timerid);
			return -1;
		}

		if( isTimerExpired(&timerid) )
		{
			msg->err=errno;
			timerDelete(&timerid);
			return 0; /* return 0 here because no bytes returned */
		}

		rt_sleep_us( sleeptime );
	}

	timerDelete(&timerid);
	return ret;
}
