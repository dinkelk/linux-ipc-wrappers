#include "SemaphoreWrapper.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define TEST_KEY 1
#define NUMSEMSIZETEST 10
#define SEMNUM 0
#define VAL 5

int main()
{
	int ret, semid;
	
	printf("Creating %d semaphores with key %d\n", NUMSEMSIZETEST, TEST_KEY);
	semid = semCreate(TEST_KEY, NUMSEMSIZETEST);
	printf("Got semid: %d\n", semid);

	printf("Getting %d semaphores with key %d\n", NUMSEMSIZETEST, TEST_KEY);
	semid = semGet(TEST_KEY, NUMSEMSIZETEST);
	printf("Got semid: %d\n", semid);

	printf("Setting semaphore %d to %d\n", SEMNUM, VAL);
	ret = semSetValue(semid, SEMNUM, VAL);
	printf("returned: %d\n", ret);

	printf("Getting semaphore: %d\n", SEMNUM);
	ret = semGetValue(semid, SEMNUM);
	printf("returned: %d\n", ret);

	printf("Signaling semaphore %d with increment value of 2\n", SEMNUM);
	ret = semIncrement(semid, SEMNUM, 2);
	printf("returned: %d\n", ret);

	printf("Getting semaphore: %d\n", SEMNUM);
	ret = semGetValue(semid, SEMNUM);
	printf("returned: %d\n", ret);

	printf("Waiting on semaphore: %d\n", SEMNUM);
	ret = semWait(semid, SEMNUM);
	printf("returned: %d\n", ret);

	printf("Getting semaphore: %d\n", SEMNUM);
	ret = semGetValue(semid, SEMNUM);
	printf("returned: %d\n", ret);

	printf("Setting semaphore %d to %d\n", SEMNUM, 0);
	ret = semSetValue(semid, SEMNUM, 0);
	printf("returned: %d\n", ret);

	printf("Getting semaphore: %d\n", SEMNUM);
	ret = semGetValue(semid, SEMNUM);
	printf("returned: %d\n", ret);

	printf("Waiting on with timeout on locked semaphore: %d\n", SEMNUM);
	ret = semWait_timeout(semid, SEMNUM, 100000);
	printf("returned: %d (-1 means the semaphore timed out)\n", ret);

	printf("Deleting semaphores... \n");
	ret = semDelete(semid);
	printf("returned: %d\n", ret);

	return 0;
}
