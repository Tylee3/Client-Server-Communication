#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    // create server socket
    int serverSocketID = socket(AF_INET, SOCK_STREAM, 0);

    // define server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9001);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int connectStatus = connect(serverSocketID, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (connectStatus == -1)
    {
        printf("Error: Could not connect to the server. \n");
    }

    // if connection is successful, proceed to send filename request
    else
    {
        if (argc < 2)
        {
            printf("Usage %s <filename>\n", argv[0]);
            return 1;
        }

        const char *filename = argv[1];
        send(serverSocketID, filename, strlen(filename) + 1, 0);

        char serverMsg[255];
        int recieved;
        while ((recieved = recv(serverSocketID, serverMsg, sizeof(serverMsg) - 1, 0)) > 0)
        {
            serverMsg[recieved] = '\0';
            printf("%s", serverMsg);
        }

        printf("\n");
    }

    close(serverSocketID);
    return 0;
}