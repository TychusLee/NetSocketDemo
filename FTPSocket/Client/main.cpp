#include <iostream>
#include "Socket.h"
#include <cstring>
#include <fstream>

using namespace std;

int main() {
    Socket s;
    char ip[10] = "127.0.0.1";
    if (s.sConnect(ip, 12128)==-1){
        return -1;
    }
    //读入文件路径并发送
    char filePath[200];
    bzero(filePath,sizeof(filePath));
    cin >> filePath;
    s.sendStr(filePath, strlen(filePath));
    //读取第一次返回数据
    int num = s.recvStr();
    const char *data = s.getStr();
    int len;
    //约定前4字节为文件长度
    memcpy(&len, data, 4);
    ofstream file;
    //约定长度为-1时代表文件不存在
    if (len == -1){
        cout<<"file not exist"<<endl;
        s.sClose();
        return 0;
    }
    //读入文件存放位置
    cin >> filePath;
    file.open(filePath, ios::binary | ios::out);
    //长度可能和文件数据都在缓存中被读出，拆包
    if (num > 4) {
        const char *str = data + 4;
        file.write(str, num - 4);
        len -= num - 4;
    }
    //大于缓存的文件循环读入
    while (len > 0) {
        num = s.recvStr();
        data = s.getStr();
        file.write(data, num);
        len -= num;
    }
    //关闭文件和连接
    s.sClose();
    file.close();
    return 0;
}