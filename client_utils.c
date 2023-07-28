#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include "utils.h"

void *recv_handler(void *arg)
{
    int socket = *(int *)arg;

    while (1)
    {
        // receive msg
        char *msg = recv_msg(socket);

        if (strcmp(msg, "EXIT\n") == 0)
        {
            printf("Server disconnected\n");
            close(socket);
            exit(EXIT_SUCCESS);
        }

        char *username, *message;

        char *command = strtok(msg, " ");

        if (command == NULL)
        {
            return NULL;
        }

        username = strtok(NULL, " ");
        message = strtok(NULL, "");

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        printf("%d-%02d-%02d %02d:%02d:%02d FROM: %s MESG: %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, username, message);
    }

    free(arg);
    return NULL;
}

void *send_handler(void *arg)
{
    int socket = *(int *)arg;

    while (1)
    {
        char *msg = (char *)malloc(1024 * sizeof(char));
        // printf("Enter your message: \n");
        fgets(msg, 1024, stdin);

        // add \n in end of msg

        send_msg(socket, msg);

        if (strcmp(msg, "EXIT\n") == 0)
            exit(EXIT_SUCCESS);
    }

    free(arg);
    return NULL;
}

pthread_t create_receiving_thread(int socket)
{
    // Create a new thread to handle commands from the client
    pthread_t tid;
    int *arg = malloc(sizeof(*arg));
    *arg = socket;
    if (pthread_create(&tid, NULL, recv_handler, arg) != 0)
    {
        perror("Error creating thread\n");
        exit(EXIT_FAILURE);
    }

    return tid;
}

pthread_t create_sending_thread(int socket)
{
    // Create a new thread to handle commands from the client
    pthread_t tid;
    int *arg = malloc(sizeof(*arg));
    *arg = socket;
    if (pthread_create(&tid, NULL, send_handler, arg) != 0)
    {
        perror("Error creating thread\n");
        exit(EXIT_FAILURE);
    }

    return tid;
}

int client_socket_setup(char *ip_address, int port)
{
    struct sockaddr_in server_add;
    struct sockaddr_in client_add;

    // Creating client socket
    int socket_binding = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_binding < 0)
    {
        perror("Client socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    // Clearing up server_add
    memset(&server_add, 0, sizeof(server_add));
    server_add.sin_addr.s_addr = htonl(INADDR_ANY);
    server_add.sin_family = AF_INET;
    server_add.sin_port = htons(port);

    // converting command line argument ip address to IPv4 format and stores it in server_add.sin_addr
    if (inet_pton(AF_INET, ip_address, &server_add.sin_addr) < 0)
    {
        perror("Conversion of string IP address to IPv4 failed\n");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(socket_binding, (struct sockaddr *)&server_add, sizeof(server_add)) < 0)
    {
        perror("Connection failed:\n");
        exit(EXIT_FAILURE);
    }

    // Print the client and server IP address and port
    socklen_t sock_size = sizeof(client_add);
    if (getsockname(socket_binding, (struct sockaddr *)&client_add, &sock_size) == -1)
    {
        perror("Client IP address access failed\n");
        exit(EXIT_FAILURE);
    }
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_add, client_ip, sock_size);

    if (getpeername(socket_binding, (struct sockaddr *)&server_add, &sock_size) < 0)
    {
        perror("Server IP address access failed\n");
        exit(EXIT_FAILURE);
    }
    char server_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &server_add, server_ip, sock_size);

    printf("%s:%d is connected to %s:%d\n", client_ip, ntohs(client_add.sin_port), server_ip, ntohs(server_add.sin_port));

    return socket_binding;
}
