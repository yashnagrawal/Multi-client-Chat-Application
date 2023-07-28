#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "utils.h"

void client(char *ip_address, int port)
{
    int socket_binding = client_socket_setup(ip_address, port);

    // Rest of the client code (main function in the original client.c)
    // ...

    // For example:
    char *username = (char *)malloc(1024 * sizeof(char));
    printf("Enter your username: \n");
    fgets(username, 1024, stdin);

    send_msg(socket_binding, username);

    // Create threads for sending and receiving messages
    pthread_t sending_thread = create_sending_thread(socket_binding);
    pthread_t receiving_thread = create_receiving_thread(socket_binding);

    pthread_join(sending_thread, NULL);
    pthread_join(receiving_thread, NULL);
}
