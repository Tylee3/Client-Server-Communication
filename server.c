#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char const *arg[])
{
    // create server socket
    int serverSocketID = socket(AF_INET, SOCK_STREAM, 0);

    // string store data to be sent to client
    char serverMsg[255] = "Server -> Client: Hello Client";

    // define server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9001);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // bind socket to the specified port and IP
    bind(serverSocketID, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    printf("Listening for Connections\n");
    // listen for connections
    // second parameter that this server will keep a backlog of 1 request
    listen(serverSocketID, 1);

    // set infinite loop for server to accept connections from clients
    while (1)
    {
        printf("Waiting for next request from Client\n");
        // define variable to hold client socket
        int clientSocket = accept(serverSocketID, NULL, NULL);

        // receive the filename requested by the client
        char filename[255];
        int recieved = recv(clientSocket, filename, sizeof(filename) - 1, 0);
        if (recieved > 0)
        {
            filename[recieved] = '\0';
            FILE *fp = fopen(filename, "r");
            if (!fp)
            {
                char *errorMsg = "Error: File not found\n";
                send(clientSocket, errorMsg, strlen(errorMsg), 0);
            }
            else
            {
                fseek(fp, 0, SEEK_END);
                long filesize = ftell(fp);
                rewind(fp);

                if (filesize > 255)
                {
                    char *errorMsg = "Error: File over 255 character limit\n";
                    send(clientSocket, errorMsg, strlen(errorMsg), 0);
                    fclose(fp);
                    close(clientSocket);
                    printf("File too big\n");
                    continue;
                }
                else
                {
                    char buffer[256];
                    int totalRead = fread(buffer, 1, filesize, fp);
                    buffer[totalRead] = '\0';
                    send(clientSocket, buffer, strlen(buffer), 0);
                }

                fclose(fp);
            }
        }

        close(clientSocket);
        printf("Responded to client correctly\n");
    }

    return 0;
}