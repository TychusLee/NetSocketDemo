#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <csignal>
#include "ServerSocket.h"
#include "Socket.h"

using namespace std;

int main() {
    int flag = 1;
    ServerSocket ss(12127);
    if (ss.sListen()) {
        return 1;
    }
    int pid = 0;
    while (flag) {
        Socket *clientSock = ss.sAccept();
        //多进程
        pid = fork();
        if (!pid) {
            //读取当前系统时间并发送
            time_t serverTime = time(0);
            clientSock->sendStr((char*)&serverTime, 4);
            //子进程关闭
            clientSock->sClose();
            break;
        }
        else{
            //主进程关闭
            clientSock->sClose();
        }
    }
    if (pid) ss.sClose();
    return 0;
}