#include "SerialWrapper.h"

int main()
{
	/*
	 * To run this test you will need at least 4 terminals open:
	 * 1)	install socat and run: socat -d -d pty,raw,echo=1 pty,raw,echo=1
	 * 2)	should get output similiar to:
	 *	 		2013/03/10 19:26:36 socat[19562] N PTY is /dev/pts/3
	 * 			2013/03/10 19:26:36 socat[19562] N PTY is /dev/pts/4
	 *			2013/03/10 19:26:36 socat[19562] N starting data transfer loop with FDs [3,3] and [5,5]
	 * 3)	set port to '/dev/pts/3' below on line 24
	 * 4)   install minicom and run: minicom -s
	 * 5) 		configure port to read on '/dev/pts/4'
	 * 6)   compile and run: make clean && make && ./SerialWrapperTest -> make sure all tests pass in terminal,
	 *      then it should be waiting to read
	 * 7)   test write: make sure data is successfully sent to minicom. should look something like this:
	 *		lots of awesome data                    
     *                          this is a string    
     *                                          Hi!
     * 8)   test read: run: echo "this is more than ten bytes" >> /dev/pts/4
     * 9)   make sure that SerialWrapperTest has recieved 10 bytes of data and exited successfully 
     * 10)  Rejoice! 
	 *
	 */
		
	int baud = 9600;
	int fd;
	int ret;
	char read_data[11];
	char data1[] = "I am writing data to an awesome serial port!";
	char data2[] = " Now give me some data to send back: ";
	int len;
	char *port;
	size_t  n = 1024;

	memset(read_data, 0, 11);
		
	printf("To run this example you will need to open up another terminal first and run:\n\n");
	printf("socat -d -d pty,raw,echo=1 -\n\n");
	printf("Socat will allow you to attach a serial file (/dev/pts) to standard out. If you do not have socat, please install it (ie. sudo apt-get install socat).\n\n");
	printf("Running this command should produce output similiar to the following:\n\n");
	printf(">> socat -d -d pty,raw,echo=1 -\n");
	printf("2015/07/04 11:34:47 socat[9979] N PTY is /dev/pts/8\n");
	printf("2015/07/04 11:34:47 socat[9979] N reading from and writing to stdio\n");
	printf("2015/07/04 11:34:47 socat[9979] N starting data transfer loop with FDs [3,3] and [0,1]\n");
	printf("\nWhat serial port did socat open? (In the case above, socat opened '/dev/pts/8'): ");
    port = malloc(n);
    if( getline(&port, &n, stdin) < 0 ) {
    	printf("Error! Reading form standard in failed!\n");
		return -1;
    }
    port[strlen(port)-1] = '\0';

	printf("\vOpening port: %s with baud %d\n", port, baud);
	if( (fd = serialOpen(port, baud)) < 0)
	{      
		printf("Error! Could not open port! Make sure socat is running and you typed in the correct port!\n");
		return -1;
	}
	else{
		printf("Serial opened successfully with file descriptor: %d\n", fd );
	}
	printf("\n");

	printf("Flushing read port...\n");
	ret = serialFlushRead(fd);
	printf("Flushing port returned %d\n", ret );

	printf("Flushing write port...\n");
	ret = serialFlushWrite(fd);
	printf("Flushing port returned %d\n", ret );

	len = strlen(data1);
	printf("Writing string '%s' to port with length %d...\n", data1, len);
	ret = serialWrite(fd,data1,len);
	printf("writing returned %d\n", ret );

	len = strlen(data2);
	printf("Writing string '%s' to port with length %d...\n", data2, len);
	ret = serialWrite(fd,(char*) data2,len);
	printf("Writing returned %d\n", ret );

	printf("\nOK. Please check your 'socat' terminal. You should see some text that was written to it though the serial wrapper.\n");
	printf("Now it is time to test reading from serial. Please type in some characters followed by [ENTER] into the 'socat' terminal.\n");
	printf("Reading up to 10 bytes from port...\n");
	while( 1 )
	{
		ret = serialRead(fd,read_data,10);
		if( ret > 0 )
			break;
	}
	read_data[strlen(read_data)-1] = '\0';
	printf("Read %d bytes of data: %s\n", ret, read_data);

	printf("\nFlushing port...\n");
	ret = serialFlush(fd);
	printf("Flushing port returned %d\n", ret );

	printf("Closing port...\n");
	ret = serialClose(fd);
	printf("Closing port returned %d\n", ret );

	return 0;
}
