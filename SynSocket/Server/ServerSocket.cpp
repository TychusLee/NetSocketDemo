//
// Created by lizz on 19-3-31.
//

#include "ServerSocket.h"
#include "Socket.h"
#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include <unistd.h>

using namespace std;

ServerSocket::ServerSocket(unsigned short port) {
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
}

//服务器监听
int ServerSocket::sListen() {
    if ((serverSock = socket(serverAddr.sin_family, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return 1;
    }
    int reuse = 1;
    //设置端口重用
    setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
    if (bind(serverSock, (sockaddr *) &serverAddr, sizeof(sockaddr))) {
        perror("bind");
        return 1;
    }
    if (listen(serverSock, WAIT_QUE_SIZE)) {
        perror("listen");
        return 1;
    }
    return 0;
}

//有连接到达构造Socket
Socket *ServerSocket::sAccept() {
    socklen_t sin_size = sizeof(struct sockaddr_in);
    sockaddr_in clientAddr;
    bzero(&clientAddr, sizeof(clientAddr));
    int client = accept(serverSock, (sockaddr *) &clientAddr, &sin_size);
    Socket *clientSocket = new Socket(client);
    return clientSocket;
}

//服务器关闭
int ServerSocket::sClose() {
    if (close(serverSock)) {
        perror("close");
        return -1;
    }
    return 0;
}