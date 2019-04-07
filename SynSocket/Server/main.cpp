#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include "ServerSocket.h"
#include "Socket.h"

using namespace std;


int main() {
    ServerSocket ss(12121);
    ss.sListen();
    int num = 0;
    //客户端连接
    Socket *clientSock = ss.sAccept();
    //读取系统时间并发送
    time_t serverTime = time(0);
    clientSock->sendStr((char*)&serverTime, 4);
    clientSock->sClose();
    ss.sClose();
    return 0;
}