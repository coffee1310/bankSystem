#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>

#include "main_menu.h"

class connection
{
public:

    static int send_msg(std::string msg);
    static std::string WINAPI get_msg();
};

#endif // CONNECTION_H
