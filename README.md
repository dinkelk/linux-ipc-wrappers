# linux-ipc-wrappers
A set of simple C libraries that simplify the use of Linux message queues, semaphores, shared memory, sockets, timers, and even serial ports.

The wrappers provided include:

* **MessageQueueWrapper.c** - a simple wrapper around System V message queues
* **SemaphoreWrapper.c** - a simple wrapper around Linux semaphores
* **SerialWrapper.c** - a simple wrapper which allows reading and writing from serial ports with all flow control turned off
* **SharedMemoryWrapper.c** - a simple wrapper around Linux shared memory
* **SocketWrapper.c** - a simple wrapper for performing IPv4 TCP socket operations
* **TimerWrapper.c** - a simple wrapper around linux time operations, providing a one-shot timer, a periodic timer, and a stop watch like timer


### Compilation
run: `make`

### Examples
Examples on how to use these libraries can be found in the `examples/` directory. Compiling the source will create 4 examples programs:

* `messageQueueWrapperTest` - This example creates a System V message queue that connects two message queue ids. A message is sent between the two ids.
* `semaphoreWrapperTest` - This example creates a set of semaphores and performs various actions on them.
* `socketWrapperTest` - This example sends an HTTP request to a server and returns the resultant HTML.
* `serialWrapperTest` - This example requires the program `socat` which can be installed via `apt-get` or similiar. `socat` is used to connect a `/dev/pts/#` to `stdout` simulating a serial port connection. The example sends data through the simulated serial port and then receives data given by the user.

### Contribution
These wrappers mostly serve as a reminder to me of how to use the various Linux IPC functions. If you find a bug, or want to improve what I have here, please submit a pull request.

Enjoy!
