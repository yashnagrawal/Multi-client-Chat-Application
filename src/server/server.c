#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "utils.h"

extern int client_count;

void *client_handler(void *arg);

void server(int port)
{
    int socket_binding = server_socket_setup(port);

    while (1)
    {
        if (client_count < MAX_CLIENTS)
        {
            int socket_com = client_acceptor(socket_binding);
            create_thread(socket_com);
        }
    }

    return;
}
