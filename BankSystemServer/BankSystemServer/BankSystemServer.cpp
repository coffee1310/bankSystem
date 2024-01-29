#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>

#pragma comment(lib,"Ws2_32.lib")

SOCKET Connections[100];
int Countet = 0;

int main()
{
    WSADATA wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        std::cout << "ERROR" << std::endl;
        exit(1);
    }

    SOCKADDR_IN addr;
    int size_addr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
    listen(sListen, SOMAXCONN);

    SOCKET newConnection;
    for (int i = 0; i < 100; i++) {
        newConnection = accept(sListen, (SOCKADDR*)&addr, &size_addr);
        char msg[256];
        if (newConnection == 0) {
            std::cout << "ERROR. Connection lost" << std::endl;
        }
        else {
            recv(newConnection, msg, sizeof(msg), NULL);
            std::cout << msg << std::endl;
        }

        Connections[i] = newConnection;
        Countet++;
    }
    
    return 0;
}
