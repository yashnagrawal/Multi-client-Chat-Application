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

TrieNode *root;
pthread_mutex_t file_lock;

int client_sockets[MAX_CLIENTS];
char *client_usernames[MAX_CLIENTS];

char *log_file = "bin/log.txt";
int client_count = 0;

TrieNode *getTrieNode()
{
    TrieNode *root = (TrieNode *)malloc(sizeof(TrieNode));
    for (int i = 0; i < no_of_alphabets; i++)
    {
        root->children[i] = NULL;
    }
    root->socket_no = -1;
    return root;
}

void insert(TrieNode *root, char *str, int n, int socket_no)
{
    TrieNode *curr_node = root;
    for (int i = 0; i < n; i++)
    {
        if (curr_node->children[str[i]] == NULL)
        {
            curr_node->children[str[i]] = getTrieNode();
        }
        curr_node = curr_node->children[str[i]];
    }
    curr_node->socket_no = socket_no;
}

int getSocketNumber(TrieNode *root, char *str, int n)
{
    TrieNode *curr_node = root;
    for (int i = 0; i < n; i++)
    {
        if (curr_node->children[str[i]] == NULL)
            return -1;
        curr_node = curr_node->children[str[i]];
    }
    return curr_node->socket_no;
}

void delete(TrieNode *root, char *str, int n)
{
    TrieNode *curr_node = root;
    for (int i = 0; i < n; i++)
    {
        if (curr_node->children[str[i]] == NULL)
            return;
        curr_node = curr_node->children[str[i]];
    }
    curr_node->socket_no = -1;
}

void *client_handler(void *arg)
{
    int socket_com = *(int *)arg;

    char *client_username = recv_username_and_store(socket_com);

    printf("Client count: %d\n", client_count);

    while (1)
    {
        char *msg = recv_msg(socket_com);
        if (strcmp(msg, "EXIT\n") == 0)
        {
            client_count--;
            printf("Client %s disconnected\n", client_username);
            printf("Client count: %d\n", client_count);
            delete (root, client_username, strlen(client_username));
            close(socket_com);
            return NULL;
        }

        if (strncmp(msg, "MESG", 4) == 0)
        {
            mesg_command_handler(msg, client_username);
            continue;
        }
    }
    free(arg);
    return NULL;
}

// function to handle MESG command in client_handler
void mesg_command_handler(char *msg, char *client_username)
{
    char *username, *message, *msg_to_send;
    msg_to_send = (char *)malloc(1024 * sizeof(char));

    char *command = strtok(msg, " ");

    if (command == NULL)
    {
        return;
    }

    username = strtok(NULL, " ");

    message = strtok(NULL, "");
    if (username == NULL || message == NULL)
    {
        printf("Invalid command\n");
        return;
    }

    int receiver_socket = getSocketNumber(root, username, strlen(username));

    if (receiver_socket == -1)
    {
        printf("Invalid username request from %s\n", client_username);
        return;
    }

    // add /0 to message
    message[strlen(message) - 1] = '\0';

    msg_to_send = strcat(msg_to_send, "MESG ");
    msg_to_send = strcat(msg_to_send, client_username);
    msg_to_send = strcat(msg_to_send, " ");
    msg_to_send = strcat(msg_to_send, message);

    send_msg(receiver_socket, msg_to_send);

    // lock file
    pthread_mutex_lock(&file_lock);

    FILE *fp = fopen(log_file, "a");

    // print from username to username in file with time, date and message
    // format: 2020-10-10 10:10:10 FROM: <sender> TO: <receiver> MESG: <message>
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(fp, "%d-%02d-%02d %02d:%02d:%02d FROM: %s TO: %s MESG: %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour,
            tm.tm_min, tm.tm_sec, client_username, username, message);

    // print to stdout
    printf("%d-%02d-%02d %02d:%02d:%02d FROM: %s TO: %s MESG: %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour,
           tm.tm_min, tm.tm_sec, client_username, username, message);

    fclose(fp);

    // unlock file
    pthread_mutex_unlock(&file_lock);
}

char *recv_username_and_store(int socket_com)
{
    // Receive response and note down client's socket details
    char *client_username = recv_msg(socket_com);

    // add /0 to client_username
    client_username[strlen(client_username) - 1] = '\0';
    client_count++;

    if (root == NULL)
        root = getTrieNode();

    insert(root, client_username, strlen(client_username), socket_com);

    printf("Client %s connected\n", client_username);

    return client_username;
}

pthread_t create_thread(int socket_com)
{
    // Create a new thread to handle commands from the client
    pthread_t tid;
    int *arg = malloc(sizeof(*arg));
    *arg = socket_com;
    if (pthread_create(&tid, NULL, client_handler, arg) != 0)
    {
        perror("Error creating thread\n");
        exit(EXIT_FAILURE);
    }

    return tid;
}

int client_acceptor(int socket_binding)
{
    struct sockaddr_in client_add;
    // Accept a connection
    socklen_t client_add_len = sizeof(client_add);
    int socket_com = accept(socket_binding, (struct sockaddr *)&client_add, &client_add_len);
    if (socket_com < 0)
    {
        perror("Accepting connection failed\n");
        exit(EXIT_FAILURE);
    }
    return socket_com;
}

int server_socket_setup(int port)
{
    int socket_binding;
    struct sockaddr_in server_add;

    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        client_sockets[i] = 0;
    }

    // Creating server socket
    socket_binding = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_binding < 0)
    {
        perror("Server socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    // Clearing up server_add
    memset(&server_add, 0, sizeof(server_add));
    server_add.sin_addr.s_addr = htonl(INADDR_ANY);
    server_add.sin_family = AF_INET;
    server_add.sin_port = htons(port);

    // Bind the socket to the address and port specified in server_add
    if (bind(socket_binding, (struct sockaddr *)&server_add, sizeof(server_add)) < 0)
    {
        perror("Binding failed\n");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(socket_binding, 1) < 0)
    {
        perror("Listening failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Server started listening on port number:%d\n", port);

    return socket_binding;
}
