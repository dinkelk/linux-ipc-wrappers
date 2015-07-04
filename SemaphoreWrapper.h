/* for linux , semtimedop() declaration */
#define __USE_GNU
#define _GNU_SOURCE
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

union semun
{
  int val;               /* value for SETVAL */
  struct semid_ds *buf;  /* buffer for IPC_STAT, IPC_SET */
  ushort  *array;        /* array for GETALL, SETALL */
                         /* Linux specific part: */
  struct seminfo *__buf; /* buffer for IPC_INFO */ 
};

/* Create num_of_sem semaphores assoc. with the path. */
/* Returns the semid */
int semCreate(key_t key, int num_of_sem);

/* Returns the semaphore ID for the path specified */
int semGet(key_t key, int number_of_sem);

/* Delete the semaphores associated with semid */
int semDelete(int semid);

/* Sets semaphore_number assoc. with the semid to value */
int semSetValue(int semid, int semaphore_number, int value);

/* Gets the semaphore value from semaphore_number assoc. with the semid */
int semGetValue(int semid, int semaphore_number);

/* Increment semaphore value by one */
int semSignal(int semid, int semaphore_number);
int semIncrement(int semid, int semaphore_number, int incrementBy);

/* Decrement semaphore value by one */
int semWait(int semid, int semaphore_number);
int semDecrement(int semid, int semaphore_number, int decrementBy);

/* Increment semaphore value by one */                 
int semSignal_timeout(int semid, int semaphore_number, long usecs);
int semIncrement_timeout(int semid, int semaphore_number, int incrementBy, long usecs);

/* Decrement semaphore value by one */     
int semWait_timeout(int semid, int semaphore_number, long usecs);
int semDecrement_timeout(int semid, int semaphore_number, int decrementBy, long usecs);