#include <stdio.h>      // Import for `printf` & `perror` functions
#include <errno.h>      // Import for `errno` variable
#include <fcntl.h>      // Import for `fcntl` functions
#include <unistd.h>     // Import for `fork`, `fcntl`, `read`, `write`, `lseek, `_exit` functions
#include <sys/types.h>  // Import for `socket`, `bind`, `listen`, `connect`, `fork`, `lseek` functions
#include <sys/socket.h> // Import for `socket`, `bind`, `listen`, `connect` functions
#include <netinet/ip.h> // Import for `sockaddr_in` stucture
#include <string.h> 

void connection_handler(int sockFD); // Handles the read & write operations to the server

#include <string.h>

void bzero(void *s, size_t len) {
    memset(s, 0, len);
}


int main(){
    int socketFd, connectStatus;
    struct sockaddr_in serverAddress, clintAddress;

    // char buf[1024];

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFd == -1){
        perror("Error while creating socket client side!");
        _exit(0);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8081);

    //connect client socket sd to server
    connectStatus = connect(socketFd, (struct sockaddr *)(&serverAddress), sizeof(serverAddress));
    if(connectStatus == -1){
        perror("Error while conneting the client!");
        close(socketFd);
        _exit(0);
    }

    // write(socketFd, "Hello Server\n", 14);
    // read(socketFd, buf, sizeof(buf));
    // printf("meassage from server: %s\n",buf);

    connection_handler(socketFd);

    close(socketFd);
}

void connection_handler(int sockFD)
{
    char readBuffer[1000], writeBuffer[1000]; // A buffer used for reading from / writting to the server
    ssize_t readBytes, writeBytes;            // Number of bytes read from / written to the socket

    char tempBuffer[1000];

    do
    {
        bzero(readBuffer, sizeof(readBuffer)); // Empty the read buffer
        bzero(tempBuffer, sizeof(tempBuffer));
        readBytes = read(sockFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
            perror("Error while reading from client socket!");
        else if (readBytes == 0)
            printf("No error received from server! Closing the connection to the server now!\n");
        else if (strchr(readBuffer, '^') != NULL)
        {
            // Skip read from client
            strncpy(tempBuffer, readBuffer, strlen(readBuffer) - 1);
            printf("%s\n", tempBuffer);
            writeBytes = write(sockFD, "^", strlen("^"));
            if (writeBytes == -1)
            {
                perror("Error while writing to client socket!");
                break;
            }
        }
        else if (strchr(readBuffer, '$') != NULL)
        {
            // Server sent an error message and is now closing it's end of the connection
            strncpy(tempBuffer, readBuffer, strlen(readBuffer) - 2);
            printf("%s\n", tempBuffer);
            printf("Closing the connection to the server now!\n");
            break;
        }
        else
        {
            bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer

            if (strchr(readBuffer, '#') != NULL)
                strcpy(writeBuffer, getpass(readBuffer));
            else
            {
                printf("%s\n", readBuffer);
                scanf("%[^\n]%*c", writeBuffer); // Take user input!
            }

            writeBytes = write(sockFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing to client socket!");
                printf("Closing the connection to the server now!\n");
                break;
            }
        }
    } while (readBytes > 0);

    close(sockFD);
}