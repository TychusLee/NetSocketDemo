//
// Created by lizz on 19-3-31.
//

#include "Socket.h"
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>

using namespace std;

//初始化缓存和缓存
Socket::Socket() {
    bzero(&netAddr, sizeof(sockaddr));
    _buf_size = 0;
    _buf_max = 0;
    _buf = nullptr;
}

//使用已有的文件描述符构造
Socket::Socket(int _sockfd) {
    sockfd = _sockfd;
    bzero(&netAddr, sizeof(sockaddr));
    _buf_size = 0;
    _buf_max = 0;
    _buf = nullptr;
}

//初始化缓存空间
int Socket::init() {
    int size_int = sizeof(int);
    if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &_buf_max, (socklen_t *) &size_int)) {
        perror("getSockOpt");
        return -1;
    }
    delete[] _buf;
    //分配缓存
    _buf = new char[_buf_max + 1];
    bzero(_buf, _buf_max + 1);
    return 0;
}

//连接
int Socket::sConnect(char *ipAddr, unsigned short port) {
    netAddr.sin_family = AF_INET;
    netAddr.sin_port = htons(port);
    netAddr.sin_addr.s_addr = inet_addr(ipAddr);
    //创建文件描述符
    if ((sockfd = socket(netAddr.sin_family, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return -1;
    }
    if (init()) {
        return -1;
    }
    if (connect(sockfd, (sockaddr *) &netAddr, sizeof(sockaddr))) {
        perror("connect");
        return -1;
    }
    return 0;
}

//向目的端发送信息
int Socket::sendStr(char *_str, int len) {
    int sendLen;
    if ((sendLen = send(sockfd, _str, len, 0)) == -1) {
        perror("send");
        return -1;
    }
    return sendLen;
}

//接受到达的数据
int Socket::recvStr() {
    bzero(_buf, _buf_max);
    int recvLen = 0;
    //读完整个缓冲区
    if ((recvLen = recv(sockfd, _buf, _buf_max, 0)) == -1) {
        perror("recv");
        return -1;
    }
    //更新缓存大小
    _buf_size = recvLen;
    return _buf_size;
}

//返回缓存指针
const char *Socket::getStr() {
    return _buf;
}

//关闭socket
int Socket::sClose() {
    if (close(sockfd)) {
        perror("close");
        return -1;
    }
    return 0;
}
