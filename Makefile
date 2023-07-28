CC = gcc
CFLAGS = -pthread

all: main

main: main.o server.o server_utils.o client.o client_utils.o utils.o
	$(CC) $(CFLAGS) -o main main.o server.o server_utils.o client.o client_utils.o utils.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

server_utils.o: server_utils.c
	$(CC) $(CFLAGS) -c server_utils.c

client_utils.o: client_utils.c
	$(CC) $(CFLAGS) -c client_utils.c

utils.o: utils.c
	$(CC) $(CFLAGS) -c utils.c

clean:
	rm -f main main.o server.o server_utils.o client.o client_utils.o utils.o
