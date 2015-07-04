#include <sys/msg.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <stdio.h>
#include "errno.h"
#include <string.h>
#include "Message.h"
#include "TimerWrapper.h"

/*****************************************************************
 * Class: messageQueueWrapper
 * Purpose: This class contains functions to create and delete
 *          message queues, as well as send and receive messages.
 * TODO: Consolidate this so that there is only one
 *       implementation of messageQueueSend() and messageQueueReceive().
 * Member messageQueueCreate(): Creates a message queue
 * Member messageQueueDelete(): Deletes a message queue
 * Member messageQueueSend() (3): Sends a message over a message queue.
 * Member messageQueueReceive() (3): Receives a message from a message queue.
 *****************************************************************/

/*****************************************************************
* 	Example IPC Limits:
*
*   >> ipcs -l
*
*   ------ Shared Memory Limits --------
*   max number of segments = 4096               // SHMMNI	
*   max seg size (kbytes) = 32768               // SHMMAX
*   max total shared memory (kbytes) = 8388608  // SHMALL
*   min seg size (bytes) = 1
*
*   ------ Semaphore Limits --------
*   max number of arrays = 1024                 // SEMMNI
*   max semaphores per array = 250              // SEMMSL
*   max semaphores system wide = 256000         // SEMMNS
*   max ops per semop call = 32                 // SEMOPM
*   semaphore max value = 32767
*
*   ------ Messages: Limits --------
*   max queues system wide = 1024               // MSGMNI
*   max size of message (bytes) = 65536         // MSGMAX
*   default max size of queue (bytes) = 65536    // MSGMNB
*****************************************************************/

/* Message queue utilities */
int messageQueueGet(key_t key);
int messageQueueCreate(key_t key);
int messageQueueDelete(int qid);

/* Message send: */
int messageQueueSend(int qid, Message *msg);
int messageQueueSend_nowait(int qid, Message *msg);
int messageQueueSend_timeout(int qid, Message *msg, int usec);

/* Message Receive */
int messageQueueReceive(int qid, Message *msg, long type);
int messageQueueReceive_nowait(int qid, Message *msg, long type);
int messageQueueReceive_timeout(int qid, Message *msg, int usec, long type);
