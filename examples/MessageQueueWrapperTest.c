#include "MessageQueueWrapper.h"
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	int i;
	int key = 10;
	int key2 = key + 1;
	int qid, qid2;
	Message msg, msg2;
	char buffer[MESSAGE_SIZE];
	int ret;
	int mtype=1;

	/* Fill buffers with data: */
	memset(buffer, 0, MESSAGE_SIZE);
	for( i = 0; i < 26; i++ ){
		buffer[i] = 65 + i;
	}

	/* Create message queues: */
	printf("making msq queue with key %d\n", key);
	qid = messageQueueCreate(key);
	printf("message queue returned qid %d\n", qid);
	if(qid == -1){ return -1; }

	printf("making another msq queue with same key %d\n", key);
	qid = messageQueueCreate(key);
	printf("message queue returned qid %d\n", qid);
	if(qid == -1){ return -1; }

	printf("making second msq queue with key %d\n", key2);
	qid2 = messageQueueCreate(key2);
	printf("second message queue returned qid %d\n", qid2);
	if(qid2 == -1){ return -1; }

	/* Create messages: */
	printf("creating message\n%s\n", messageToString(&msg));
	clearMessage(&msg);
	printf("clearing message\n%s\n", messageToString(&msg));
	clearMessage(&msg2);
	printf("creating second blank message\n%s\n", messageToString(&msg2));

	setMessage(&msg, buffer, MESSAGE_SIZE, mtype);
	printf("Making message with buffer:\n%s\n\n", buffer);

	/* Send message: */
	printf("Sending message from qid: %d to qid: %d\n", qid, qid2);
	ret = messageQueueSend(qid2, &msg);
	printf("Sending message returned %d\n", ret);
	printf("Sent message: %s\n", messageToString(&msg));

	/* Receive message: */
	printf("Receiving message from qid: %d\n", qid2);
	ret = messageQueueReceive(qid2, &msg2, mtype);
	printf("Receiving message returned %d\n", ret);
	printf("Received message: %s\n", messageToString(&msg2));

	/* Delete queues: */
	printf("Deleting qid: %d returned %d\n", qid , messageQueueDelete(qid));
	printf("Deleting qid: %d returned %d\n", qid2, messageQueueDelete(qid2));
	
	return 0;
}
