#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>

void connection_handler(int connFd);


int main(){
    int socketFd, socketBindStatus, socketListenStatus, connectionFileDescription;
    struct sockaddr_in serverAddress, clintAddress;

    socketFd = socket(AF_UNIX, SOCK_STREAM,0);

    if(socketFd == -1){
        pereor("Error while creating server socket!!");
        _exit(0);
    }

    serverAddress.sin_family = AF_INET;     //IPv4
    serverAddress.sin_port = htons(8080); 
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

    int clintSize;
    while(1){
        clintSize = sizeof(clintAddress);

        connectionFileDescription = accept(socketFd, (struct sockaddr*)&clintAddress, clintSize);
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
    
}