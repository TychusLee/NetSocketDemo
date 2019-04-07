#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include "Socket.h"

using namespace std;

int main() {
    Socket sock;
    char ipAddr[] = "127.0.0.1\0";
    //连接到服务器
    if (sock.sConnect(ipAddr,12121)){
        return -1;
    }
    //读取时间并输出
    sock.recvStr();
    time_t *serverTime = (time_t *)sock.getStr();
    printf("%s\n",ctime(serverTime));
    sock.sClose();
    return 0;
}