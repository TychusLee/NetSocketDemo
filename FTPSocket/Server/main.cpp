#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include "ServerSocket.h"
#include "Socket.h"

using namespace std;

int main() {
    ServerSocket ss(12128);
    ss.sListen();
    while (1) {
        Socket *sock = ss.sAccept();
        sock->init();
        //多进程
        int pid = fork();
        //子进程工作
        if (!pid) {
            int len = 0;
            //读入文件路径
            if ((len = sock->recvStr()) == -1)
                return -1;
            const char *str = sock->getStr();
            char filePath[200];
            bzero(filePath, sizeof(filePath));
            memcpy(filePath, str, len);
            //申请发送缓存
            char file_buf[4000];
            //打开文件
            ifstream file;
            file.open(filePath, ios::binary | ios::in);
            //统计并发送文件长度
            file.seekg(0, file.end);
            len = file.tellg();
            file.seekg(0, file.beg);
            if (!file.is_open()) len = -1;
            sock->sendStr((char *) &len, 4);
            int size = 0;
            //文件大于发送缓存，循环发送
            while (len > 0) {
                bzero(file_buf, sizeof(file_buf));
                //未发送部分大于缓存，一次发送缓存大小
                if (len > sizeof(file_buf)) {
                    file.read(file_buf, sizeof(file_buf));
                    size = sock->sendStr(file_buf, sizeof(file_buf));
                }
                    //文发送部分小于缓存，发送完剩余部分
                else {
                    file.read(file_buf, len);
                    size = sock->sendStr(file_buf, len);
                }
                cout << size << endl;
                len -= sizeof(file_buf);
            }
            //子进程中关闭文件和socket
            sock->sClose();
            file.close();
            delete sock;
            break;
        } else {
            //主进程关闭socket
            sock->sClose();
            delete sock;
        }
        if (!&ss) break;
    }
    return 0;
}