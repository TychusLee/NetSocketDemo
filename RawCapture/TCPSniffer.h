//
// Created by lizz on 19-4-2.
//
#define _BUF_SIZE 500
#ifndef RAWCAPTURE_TCPSNIFFER_H
#define RAWCAPTURE_TCPSNIFFER_H

#include <arpa/inet.h>

struct TCPHeader {
    unsigned short sPort;
    unsigned short dPort;
    unsigned int seq;
    unsigned int ack;
    unsigned char headerLen;
    unsigned char flag;
    unsigned short win;
    unsigned short sum;
    unsigned short urp;
};

struct IPHeader {
    unsigned char version;
    unsigned char headerLen;
    unsigned char serveType;
    unsigned short totalLen;
    unsigned short packetID;
    unsigned short sliceInfo;
    unsigned char TTL;
    unsigned char protocolType;
    unsigned short sum;
    unsigned int sIP;
    unsigned int dIP;
};

void *start(void *);

class TCPSniffer {
    int snifferfd;
    TCPHeader tcpHeader;
    IPHeader ipHeader;
    char data[_BUF_SIZE];
public:
    explicit TCPSniffer();

    int sListen();

    void start();

    int catTCP();

    int catIP();

    ~TCPSniffer();
};


#endif //RAWCAPTURE_TCPSNIFFER_H
