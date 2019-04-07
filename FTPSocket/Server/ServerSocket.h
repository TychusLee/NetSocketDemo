//
// Created by lizz on 19-3-31.
//
#define WAIT_QUE_SIZE 20
#ifndef SERVER_SERVERSOCKET_H
#define SERVER_SERVERSOCKET_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include "Socket.h"

class ServerSocket {
    sockaddr_in serverAddr;
    int serverSock;
public:
    ServerSocket(unsigned short port);
    int sListen();
    Socket *sAccept();
    int sClose();
};


#endif //SERVER_SERVERSOCKET_H
