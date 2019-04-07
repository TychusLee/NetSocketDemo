//
// Created by lizz on 19-3-31.
//

#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <arpa/inet.h>

class Socket {
    sockaddr_in netAddr;
    int sockfd;
    char *_buf;
    int _buf_size;
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
};


#endif //SERVER_SOCKET_H
