#include <iostream>
#include <cstring>
#include <unistd.h>
#include "TCPSniffer.h"
using namespace std;
int main() {
    TCPSniffer ts;
    if (ts.sListen()){
        return -1;
    }
    ts.start();
    return 0;
}