## Multi Client Chat Application using c socket programming

Overview:
The C Socket Programming code includes two main programs: `server` and `client`. The server program listens for incoming client connections, while the client program connects to the server to exchange messages. The communication is achieved through sockets, and the programs use pthreads for handling multiple clients simultaneously.

## Prerequisites:

To compile and run the C Socket Programming code, you need to have the following installed on your system:

1. GCC Compiler: Make sure you have the GNU Compiler Collection (GCC) installed to compile the C code.
   @@ -15,10 +16,12 @@ make

This will compile the source files and generate the executable named `main`.

## Usage:

The `main` executable can be run with different options to start the server or client program.

## Running the Server:

To run the server, execute the `main` executable with the following command:

./bin/main s <port>
@@ -29,7 +32,8 @@ Replace `<port>` with the desired port number to listen on for incoming client c

The server will start and listen for incoming client connections on the specified port.

## Running the Client:

To run the client, execute the `main` executable with the following command:

./bin/main c <ip_address> <port>
@@ -40,12 +44,13 @@ Replace `<ip_address>` with the IP address of the server to connect to, and `<po

The client will connect to the server using the provided IP address and port number.

## Cleaning Up:

To remove the compiled files and clean up the directory, use the following command:

make clean

## Additional Notes:

- The server program can handle multiple client connections simultaneously, as it spawns a new thread for each client.
- The client program prompts the user to enter a username before starting communication with the server.
