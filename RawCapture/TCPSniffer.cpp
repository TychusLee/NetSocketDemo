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
        //按IP过滤
        if (ipHeader.dIP != test.sin_addr.s_addr || ipHeader.sIP != test.sin_addr.s_addr) continue;
        test.sin_addr.s_addr = ipHeader.sIP;
        cout << inet_ntoa(test.sin_addr) << "->";
        test.sin_addr.s_addr = ipHeader.dIP;
        cout << inet_ntoa(test.sin_addr) << endl;
        inet_aton("127.0.0.1", &test.sin_addr);
        //分离TCP首部
        catTCP();
        //按端口过滤
        if (tcpHeader.sPort != htons(12127) && tcpHeader.dPort != htons(12127)) continue;
        cout << ntohs(tcpHeader.sPort) << "->" << ntohs(tcpHeader.dPort) << endl;
        cout << ipHeader.totalLen << endl;
        cout << ipHeader.totalLen - tcpHeader.headerLen - ipHeader.headerLen << endl;
        //按应用层数据过滤
        if (ipHeader.totalLen - tcpHeader.headerLen - ipHeader.headerLen != 4) continue;
        //输出应用层数据
        cout << "-------------------packet--------------------" << endl;
        cout << "------------------Network--------------------" << endl;
        sockaddr_in trans;
        trans.sin_addr.s_addr = ipHeader.sIP;
        cout << "     Source: " << inet_ntoa(trans.sin_addr) << endl;
        trans.sin_addr.s_addr = ipHeader.dIP;
        cout << "Destination: " << inet_ntoa(trans.sin_addr) << endl;
        cout << "--------------Transportation----------------" << endl;
        cout << "     Source: " << ntohs(tcpHeader.sPort) << endl;
        cout << "Destination: " << ntohs(tcpHeader.dPort) << endl;
        cout << "----------------Application-----------------" << endl;
        time_t serverTime;
        memcpy(&serverTime, data + ipHeader.headerLen + tcpHeader.headerLen, 4);
        cout << ctime(&serverTime) << endl;
        if (!ipHeader.headerLen) break;
    }
}

int TCPSniffer::catIP() {
    int num;
    //读入IP包
    if ((num = recv(snifferfd, data, _BUF_SIZE, 0)) == -1) {
        perror("recvIPHeader");
        return -1;
    }
    //按字节拷贝分离出IP首部
    memcpy(&ipHeader.headerLen, data, 1);
    ipHeader.version = ipHeader.headerLen >> 4;
    ipHeader.headerLen = (ipHeader.headerLen & 15) * 4;
    memcpy(&ipHeader.serveType, data + 1, 1);
    memcpy(&ipHeader.totalLen, data + 2, 2);
    ipHeader.totalLen = ntohs(ipHeader.totalLen);
    memcpy(&ipHeader.packetID, data + 4, 2);
    memcpy(&ipHeader.sliceInfo, data + 6, 2);
    memcpy(&ipHeader.TTL, data + 8, 1);
    memcpy(&ipHeader.protocolType, data + 9, 1);
    memcpy(&ipHeader.sum, data + 10, 2);
    memcpy(&ipHeader.sIP, data + 12, 4);
    memcpy(&ipHeader.dIP, data + 16, 4);
    return 0;
}

int TCPSniffer::catTCP() {
    //从TCP首部位置开始，按字节拷贝，分理出TCP首部
    char *TCPpacket = data + ipHeader.headerLen;
    memcpy(&tcpHeader.sPort, TCPpacket, 2);
    memcpy(&tcpHeader.dPort, TCPpacket + 2, 2);
    memcpy(&tcpHeader.seq, TCPpacket + 4, 4);
    memcpy(&tcpHeader.ack, TCPpacket + 8, 4);
    memcpy(&tcpHeader.headerLen, TCPpacket + 12, 1);
    tcpHeader.headerLen = tcpHeader.headerLen >> 4 << 2;
    memcpy(&tcpHeader.flag, TCPpacket + 13, 1);
    memcpy(&tcpHeader.win, TCPpacket + 14, 2);
    memcpy(&tcpHeader.sum, TCPpacket + 16, 2);
    memcpy(&tcpHeader.urp, TCPpacket + 18, 2);
    return 0;
}

TCPSniffer::~TCPSniffer() {
    close(snifferfd);
}
