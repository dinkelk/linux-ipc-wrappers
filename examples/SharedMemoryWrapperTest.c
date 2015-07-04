#include "SharedMemoryWrapper.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define SHMEMSIZETEST 10
#define TEST 6

int main()
{
	int ret, shmid;
	char* shmaddr = 0;
	char toWrite[3] = {'h','i','\0'};
	unsigned int i;

	printf("Creating shared memory with key '%d' of size %d bytes.\n", TEST, SHMEMSIZETEST);
	shmid = sharedMemoryCreate(TEST, SHMEMSIZETEST);
	printf("Got shmid: %d\n\n", shmid);

	printf("Getting shared memory with key '%d' of size %d bytes.\n", TEST, SHMEMSIZETEST);
	shmid = sharedMemoryGet(TEST, SHMEMSIZETEST);
	printf("Got shmid: %d\n\n", shmid);

	printf("Attaching shared memory to address: %p\n", shmaddr);
	shmaddr = (char*) sharedMemoryAttach(shmid);
	printf("Which now points to: %p\n\n", shmaddr);

	printf("Locking shared memory...\n");
	ret = sharedMemoryLock(shmid);
	printf("returned %d\n\n", ret);

	printf("Writing '%s' to shared memory...\n", toWrite);
	for(i = 0; i < strlen(toWrite); i++)
		*(shmaddr + i) = toWrite[i];
	printf("Reading from shared memory: '%s'\n\n", shmaddr);

	printf("Unlocking shared memory...\n");
	ret = sharedMemoryUnlock(shmid);
	printf("returned %d\n\n", ret);

	printf("Detatching shared memory from address: %p\n", shmaddr);
	ret = sharedMemoryDetatch(shmaddr);
	printf("returned %d\n", ret);
	printf("Which now points to: %p\n\n", shmaddr);

	printf("Deleting shared memory... \n");
	ret = sharedMemoryDelete(shmid);
	printf("returned: %d\n", ret);

	return 0;
}
