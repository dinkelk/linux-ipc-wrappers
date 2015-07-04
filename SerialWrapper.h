#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stropts.h>
#include <time.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <termios.h>
#include "Timer.h"

#define HALF_SECOND 500000

int serialOpen(char* port_address, int baud_rate);  
int serialFlush(int fd);
int serialFlushRead(int fd);
int serialFlushWrite(int fd);
int serialWrite(int fd, char* data, int length);
int serialRead(int fd, char* temp, int length);
int serialCommandByDelim(int fd, char* cmd, char* reply, int replyLength, int usecs, char delim);
int serialCommandByLength(int fd, char* cmd, char* reply, int replyLength, int usecs);
int serialBufferCount(int fd);
int serialClose(int fd);