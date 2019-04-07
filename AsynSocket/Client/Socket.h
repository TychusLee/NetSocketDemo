//
// Created by lizz on 19-3-31.
//

#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <arpa/inet.h>

class Socket {
    //本地地址
    sockaddr_in netAddr;
    //文件描述符
    int sockfd;
    //接收缓存
    char *_buf;
    //接收缓存中数据长度
    int _buf_size;
    //接收缓存大小
    int _buf_max;

public:
    explicit Socket(int sockfd);

    Socket();

    int init();

    int sConnect(char *, unsigned short);

    int sendStr(char *str, int len);

    int recvStr();

    const char *getStr();

    int sClose();

    ~Socket();
};


#endif //SERVER_SOCKET_H
