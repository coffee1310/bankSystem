#include "mainwindow.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <QApplication>
#include <iostream>
#include <thread>
#include <stdio.h>
#include <windows.h>


void ClientHandler() {
    int msg_size;
    while (true) {
        recv(Connection, (char*)&msg_size, sizeof(int), NULL);
        char* msg = new char[msg_size + 1];
        msg[msg_size] = '\0';
        recv(Connection, msg, msg_size, NULL);
        qDebug().noquote() << QString::fromUtf8(msg);
        delete[] msg;
    }
}

int connect_to_server() {
    WSAStartup;
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        qDebug() << "Error";
        exit(1);
    }

    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;

    Connection = socket(AF_INET, SOCK_STREAM, NULL);
    if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
        qDebug() << "Error: failed connect to server.\n";
        return 1;
    }
    qDebug() << "Connected!\n";


    return 0;
}

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(1251);

    connect_to_server();
    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    return a.exec();
}
