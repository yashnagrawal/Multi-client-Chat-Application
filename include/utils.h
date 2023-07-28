#ifndef UTILS_H
#define UTILS_H

#define MAX_CLIENTS 20

int client_socket_setup(char *ip_address, int port);
int server_socket_setup(int port);
int client_acceptor(int socket_binding);
void *client_handler(void *arg);
void mesg_command_handler(char *msg, int client_index);
int recv_username_and_store(int socket_com);
pthread_t create_thread(int socket_com);

char *get_ipaddress_from_socket(int socket_com);
void keyword_adder(char *msg, char *keyword);
void keyword_remover(char *msg, char *keyword);
void send_msg(int socket_com, char *msg);
char *recv_msg(int socket_com);

pthread_t create_receiving_thread(int socket);
pthread_t create_sending_thread(int socket);

#endif
