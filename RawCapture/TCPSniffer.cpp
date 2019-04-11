//
// Created by lizz on 19-4-2.
//

#include "TCPSniffer.h"
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

using namespace std;

TCPSniffer::TCPSniffer() {
    bzero(data, sizeof(data));
}

int TCPSniffer::sListen() {
    if ((snifferfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) == -1) {
        perror("socket");
        return -1;
    }
    bool flag = true;
    return 0;
}

void TCPSniffer::start() {
    while (1) {
        //初始化首部信息和缓存
        bzero(&ipHeader, sizeof(IPHeader));
        bzero(&tcpHeader, sizeof(TCPHeader));
        bzero(data, sizeof(data));
        //分离IP首部
        catIP();
        sockaddr_in test;
        inet_aton("127.0.0.1", &test.sin_addr);
        test.sin_addr.s_addr = ipHeader->sIP;
        cout << inet_ntoa(test.sin_addr) << "->";
        test.sin_addr.s_addr = ipHeader->dIP;
        cout << inet_ntoa(test.sin_addr) << endl;
        inet_aton("127.0.0.1", &test.sin_addr);
        //分离TCP首部
        catTCP();
        int ipHeaderLen = (ipHeader->headerLen & 15) * 4;
        int tcpHeaderLen = tcpHeader->headerLen >> 4 << 2;
        int totalLen = ntohs(ipHeader->totalLen);
        cout << ntohs(tcpHeader->sPort) << "->" << ntohs(tcpHeader->dPort) << endl;
        cout << ipHeaderLen << endl;
        cout << tcpHeaderLen << endl;
        cout << totalLen << endl;
        //按IP过滤
        if (ipHeader->dIP != test.sin_addr.s_addr || ipHeader->sIP != test.sin_addr.s_addr) continue;
        //按端口过滤
        if (tcpHeader->sPort != htons(12127) && tcpHeader->dPort != htons(12127)) continue;
        //按应用层数据过滤
        if (totalLen - tcpHeaderLen - ipHeaderLen != 4) continue;
        //输出应用层数据
        cout << "-------------------packet--------------------" << endl;
        cout << "------------------Network--------------------" << endl;
        sockaddr_in trans;
        trans.sin_addr.s_addr = ipHeader->sIP;
        cout << "     Source: " << inet_ntoa(trans.sin_addr) << endl;
        trans.sin_addr.s_addr = ipHeader->dIP;
        cout << "Destination: " << inet_ntoa(trans.sin_addr) << endl;
        cout << "--------------Transportation----------------" << endl;
        cout << "     Source: " << ntohs(tcpHeader->sPort) << endl;
        cout << "Destination: " << ntohs(tcpHeader->dPort) << endl;
        cout << "----------------Application-----------------" << endl;
        time_t serverTime;
        memcpy(&serverTime, data + ipHeaderLen + tcpHeaderLen, 4);
        cout << ctime(&serverTime) << endl;
        if (!ipHeader->headerLen) break;
    }
}

int TCPSniffer::catIP() {
    int num;
    //读入IP包
    if ((num = recv(snifferfd, data, _BUF_SIZE, 0)) == -1) {
        perror("recvIPHeader");
        return -1;
    }
    ipHeader = (IPHeader *) data;
    return 0;
}

int TCPSniffer::catTCP() {
    tcpHeader = (TCPHeader *) (data + (ipHeader->headerLen & 15) * 4);
    return 0;
}

TCPSniffer::~TCPSniffer() {
    close(snifferfd);
}
