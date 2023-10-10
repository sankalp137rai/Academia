#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>

void connection_handler(int connFd);

#include <string.h>

void bzero(void *s, size_t len) {
    memset(s, 0, len);
}


int main(){
    int socketFd, socketBindStatus, socketListenStatus, connectionFileDescription;
    struct sockaddr_in serverAddress, clintAddress;

    socketFd = socket(AF_INET, SOCK_STREAM,0);

    if(socketFd == -1){
        perror("Error while creating server socket!!");
        _exit(0);
    }

    serverAddress.sin_family = AF_INET;     //IPv4
    serverAddress.sin_port = htons(8081); 
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); //bind socke to all interfaces

    socketBindStatus = bind(socketFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    if(socketBindStatus == -1){
        perror("Error while binding the socket");
        _exit(0);
    }

    socketListenStatus = listen(socketFd, 10);

    if(socketListenStatus == -1){
        perror("Error while listening the socket!!");
        close(socketFd);
        _exit(0);
    }
    
    // int clintSize;
    // char buf[1024];
    // clintSize = sizeof(clintAddress);

    // connectionFileDescription = accept(socketFd, (struct sockaddr*)&clintAddress, &clintSize);

    // read(connectionFileDescription, buf, sizeof(buf));
    // printf("Message from client: %s\n",buf);
    // write(connectionFileDescription, "ACK from server\n", 14);

    int clintSize;
    while(1){
        clintSize = sizeof(clintAddress);

        connectionFileDescription = accept(socketFd, (struct sockaddr*)&clintAddress, &clintSize);
        if(connectionFileDescription == -1){
            perror("error while connecting to client");
            _exit(0);
        }
        else{
            if(!fork()){
                connection_handler(connectionFileDescription);
                close(connectionFileDescription);
                _exit(0);
            }
        }
    }
    close(socketFd);
}

void connection_handler(int connectionFd){
    printf("clint has connected to the server!!");
    char readBuff[1024], writeBuff[1024];
    ssize_t readBytes, writeBytes;

    int userChoice;

    writeBytes = write(connectionFd, "Ack from server", sizeof("Ack from server"));
    if (writeBytes == -1)
        perror("Error while sending first prompt to the user!");
    else
    {
        bzero(readBuff, sizeof(readBuff));
        readBytes = read(connectionFd, readBuff, sizeof(readBuff));
        if (readBytes == -1)
            perror("Error while reading from client");
        else if (readBytes == 0)
            printf("No data was sent by the client");
        else
        {
            userChoice = atoi(readBuff);
            switch (userChoice)
            {
            case 1:
                // Admin
                printf("In the admin");
                
                break;
            case 2:
                //facalty
                printf("In the facalty");
                break;
            default:
                // student
                printf("In the student");
                break;
            }
        }
    }
    printf("Terminating connection to client!\n");
}