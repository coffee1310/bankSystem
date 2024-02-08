#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>

#pragma comment(lib,"Ws2_32.lib")

SOCKET Connections[100];
int Counter = 0;

class UserDataBase {
public:
	UserDataBase(SOCKET Connection) {
		std::string msg;
		int msg_size;
		char* res;
		int res_size;

		msg = "Введите номер телефона: ";
		msg_size = msg.size();

		send(Connection, (char*)&msg_size, sizeof(int), NULL);
		send(Connection, msg.c_str(), msg_size, NULL);

		
		recv(Connection, (char*)&res_size, sizeof(int), NULL);
		res = new char[res_size + 1];
		msg[msg_size] = '\0';
		recv(Connection, res, res_size, NULL);
		
		this->phone_number = res;
		msg = "Введите пароль: ";
		msg_size = msg.size();

		send(Connection, (char*)&msg_size, sizeof(int), NULL);
		send(Connection, msg.c_str(), msg_size, NULL);
	}

private:
	std::string phone_number;
	std::string password;

	bool check_phone_number()
	{
		return false;
	}
};

std::string do_command(std::string msg, SOCKET connection)
{
	if (msg == "help") {
		std::string result = "1. help - список команд\n"
			"2. Добавить средств\n"
			"3. Перевести\n";
		return result;
	}
	if (msg == "create_user") {
		UserDataBase user(connection);
	}
	return "";
}


void ClientHandler(int index) {
    int msg_size;
    while (true) {
        recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);
        char* msg = new char[msg_size + 1];
        msg[msg_size] = '\0';
        recv(Connections[index], msg, msg_size, NULL);
		std::cout << msg << std::endl;
        for (int i = 0; i < Counter; i++) {
            if (i == index) {
				std::string result;
				int result_size;
				result = do_command(msg, Connections[i]);
				result_size = result.size();
				send(Connections[i], (char*)&result_size, sizeof(int), NULL);
				send(Connections[i], result.c_str(), result_size, NULL);
                continue;
            }

            /*send(Connections[i], (char*)&msg_size, sizeof(int), NULL);
            send(Connections[i], msg, msg_size, NULL);*/
        }
        delete[] msg;
    }
}


int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "ru");
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	for (int i = 0; i < 100; i++) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

		if (newConnection == 0) {
			std::cout << "Error #2\n";
		}
		else {
			std::cout << "Client Connected!\n";
			std::string msg = "Hello. It`s my first network program!";
			int msg_size = msg.size();
			send(newConnection, (char*)&msg_size, sizeof(int), NULL);
			send(newConnection, msg.c_str(), msg_size, NULL);

			Connections[i] = newConnection;
			Counter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
		}
	}


	system("pause");
	return 0;
}