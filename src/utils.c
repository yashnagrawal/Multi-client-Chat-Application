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

void send_msg(int socket_com, char *msg)
{
    if (send(socket_com, msg, 1024, 0) < 0)
    {
        perror("Send failed\n");
        exit(EXIT_FAILURE);
    }
    // printf("Message sent: %s\n", msg);
    // print error
}

char *recv_msg(int socket_com)
{
    char *msg_recv_by_client = (char *)malloc(1024 * sizeof(char));
    int recv_status = recv(socket_com, msg_recv_by_client, 1024, 0);

    if (recv_status < 0)
    {
        perror("recv failed\n");
        exit(EXIT_FAILURE);
    }
    else if (recv_status == 0)
    {
        return "EXIT\n";
    }
    return msg_recv_by_client;
}

char *get_ipaddress_from_socket(int socket_com)
{
    // extract ip address of client from socket_com
    struct sockaddr_in client_add;
    int len = sizeof(client_add);
    getpeername(socket_com, (struct sockaddr *)&client_add, (socklen_t *)&len);
    char *ip_address = inet_ntoa(client_add.sin_addr);
    return ip_address;
}