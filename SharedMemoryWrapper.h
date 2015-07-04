#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "errno.h"

/* Create and delete shared memory: */
int sharedMemoryGet(key_t key, int size);
int sharedMemoryCreate(key_t key, int size);
int sharedMemoryCreateOrGet(key_t key, int size);
int sharedMemoryCreateIfGone(key_t key, int size);
int sharedMemoryDelete(int shmid);

/* Attach / detach memory from process: */
void* sharedMemoryAttach(int shmid);
int sharedMemoryDetatch(const void* shmaddr);

/* Prevent shared memory from being swapped in / out: */
int sharedMemoryLock(int shmid);
int sharedMemoryUnlock(int shmid);