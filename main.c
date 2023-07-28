#include <stdio.h>
#include <stdlib.h> // for atoi()

void server(int port);
void client(char *ip_address, int port);

int main(int argc, char *argv[])
{
    if (argc < 1)
    {
        printf("Format: <option> <ip_address> <port>\n");
        return 0;
    }

    char option = argv[1][0];

    if (option == 'c')
    {
        // argc check
        if (argc < 4)
        {
            printf("Format: <option> <ip_address> <port>\n");
            return 0;
        }
        char *ip_address = argv[2];
        int port = atoi(argv[3]);

        printf("ip_address: %s\n", ip_address);
        client(ip_address, port);
    }
    else if (option == 's')
    {
        // argc check
        if (argc < 3)
        {
            printf("Format: <option> <port>\n");
            return 0;
        }
        int port = atoi(argv[2]);

        server(port);
    }
    else
    {
        int port = atoi(argv[3]);
        printf("Invalid option\n");
    }

    return 0;
}
