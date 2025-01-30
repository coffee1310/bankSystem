#include "connection.h"

int connection::send_msg(std::string msg) {
    // int msg_size = 0;
    // msg_size = msg.size();
    // send(Connection, (char*)&msg_size, sizeof(int), NULL);
    // send(Connection, msg.c_str(), msg_size, NULL);
    // return 0;
        /*int msg_size;
    int res_size;
    msg_size = msg.size();
    send(this->Connection, (char*)&msg_size, sizeof(int), NULL);
    send(this->Connection, msg.c_str(), msg_size, NULL);*/

    int msg_size = msg.size();
    int send_bytes = send(Connection, (char*)&msg_size, sizeof(int), 0);

    if (send_bytes != sizeof(int)) {
        return -1;
    }

    send_bytes = send(Connection, msg.c_str(), msg_size, 0);

    if (send_bytes != msg_size) {
        return -1;
    }

    return 0;
}

std::string WINAPI connection::get_msg() {
    // int res_size = 0;
    // recv(Connection, (char*)&res_size, sizeof(int), NULL);
    // if (res_size > 1) {
    //     char* res = new char[res_size + 1];
    //     res[res_size] = '\0';
    //     recv(Connection, res, res_size, NULL);
    //     std::string result(res);
    //     delete[] res;
    //     return result;
    // }
    // return "";
        /*int res_size;
    recv(this->Connection, (char*)&res_size, sizeof(int), NULL);
    char* res = new char[res_size + 1];
    res[res_size] = '\0';
    recv(this->Connection, res, res_size, NULL);
    std::string result(res);
    delete[] res;
    return result;*/

    int res_size = 0;
    int received_bytes = recv(Connection, (char*)&res_size, sizeof(int), 0);

    if (received_bytes <= 0) {
        return "";
    }

    if (res_size <= 0) {
        return "";
    }

    char* res = new char[res_size + 1];
    res[res_size] = '\0';

    received_bytes = recv(Connection, res, res_size, 0);
    if (received_bytes != res_size) {
        delete[] res;
        return "";
    }

    std::string result(res);
    delete[] res;
    return result;
}
