CC=gcc
CFLAGS=-O3 -pipe -Wall -Wextra -pedantic
INCLUDE=-I.
EXAMPLE_DIR=examples
OBJ=Message.o Timer.o MessageQueueWrapper.o SemaphoreWrapper.o SerialWrapper.o SharedMemoryWrapper.o SocketWrapper.o $(EXAMPLE_DIR)/MessageQueueWrapperTest.o $(EXAMPLE_DIR)/SemaphoreWrapperTest.o $(EXAMPLE_DIR)/SerialWrapperTest.o $(EXAMPLE_DIR)/SharedMemoryWrapperTest.o $(EXAMPLE_DIR)/SocketWrapperTest.o
BIN=$(EXAMPLE_DIR)/messageQueueWrapperTest $(EXAMPLE_DIR)/semaphoreWrapperTest $(EXAMPLE_DIR)/serialWrapperTest $(EXAMPLE_DIR)/sharedMemoryWrapperTest $(EXAMPLE_DIR)/socketWrapperTest

all: $(OBJ) $(BIN)

Message.o: Message.c
	$(CC) -o $@ $(CFLAGS) -c $^

Timer.o: Timer.c
	$(CC) -o $@ $(CFLAGS) -c $^

MessageQueueWrapper.o: MessageQueueWrapper.c
	$(CC) -o $@ $(CFLAGS) -c $^

SemaphoreWrapper.o: SemaphoreWrapper.c
	$(CC) -o $@ $(CFLAGS) -c $^

SharedMemoryWrapper.o: SharedMemoryWrapper.c
	$(CC) -o $@ $(CFLAGS) -c $^

SocketWrapper.o: SocketWrapper.c
	$(CC) -o $@ $(CFLAGS) -c $^

SerialWrapper.o: SerialWrapper.c
	$(CC) -o $@ $(CFLAGS) -c $^

$(EXAMPLE_DIR)/MessageQueueWrapperTest.o: $(EXAMPLE_DIR)/MessageQueueWrapperTest.c
	$(CC) -o $@ $(CFLAGS) -c $(INCLUDE) $^

$(EXAMPLE_DIR)/messageQueueWrapperTest: $(EXAMPLE_DIR)/messageQueueWrapperTest.o MessageQueueWrapper.o Timer.o Message.o
	$(CC) -o $@ $(CFLAGS) $^ -lrt

$(EXAMPLE_DIR)/SemaphoreWrapperTest.o: $(EXAMPLE_DIR)/SemaphoreWrapperTest.c
	$(CC) -o $@ $(CFLAGS) -c $(INCLUDE) $^

$(EXAMPLE_DIR)/semaphoreWrapperTest: $(EXAMPLE_DIR)/semaphoreWrapperTest.o SemaphoreWrapper.o
	$(CC) -o $@ $(CFLAGS) $^

$(EXAMPLE_DIR)/SerialWrapperTest.o: $(EXAMPLE_DIR)/SerialWrapperTest.c
	$(CC) -o $@ $(CFLAGS) -c $(INCLUDE) $^

$(EXAMPLE_DIR)/serialWrapperTest: $(EXAMPLE_DIR)/SerialWrapperTest.o SerialWrapper.o Timer.o
	$(CC) -o $@ $(CFLAGS) $^ -lrt

$(EXAMPLE_DIR)/SharedMemoryWrapperTest.o: $(EXAMPLE_DIR)/SharedMemoryWrapperTest.c
	$(CC) -o $@ $(CFLAGS) -c $(INCLUDE) $^

$(EXAMPLE_DIR)/sharedMemoryWrapperTest: $(EXAMPLE_DIR)/SharedMemoryWrapperTest.o SharedMemoryWrapper.o
	$(CC) -o $@ $(CFLAGS) $^ -lrt

$(EXAMPLE_DIR)/SocketWrapperTest.o: $(EXAMPLE_DIR)/SocketWrapperTest.c
	$(CC) -o $@ $(CFLAGS) -c $(INCLUDE) $^

$(EXAMPLE_DIR)/socketWrapperTest: $(EXAMPLE_DIR)/SocketWrapperTest.o SocketWrapper.o
	$(CC) -o $@ $(CFLAGS) $^ -lrt

clean: 
	-rm -f $(OBJ) $(BIN)