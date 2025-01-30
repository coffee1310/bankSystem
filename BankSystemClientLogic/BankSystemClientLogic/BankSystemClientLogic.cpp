#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "SimpleIni.h"

#pragma comment(lib,"Ws2_32.lib")

SOCKET Connection;

int send_msg(std::string msg) {
	int msg_size;
	char* res;
	int res_size;
	msg_size = msg.size();
	send(Connection, (char*)&msg_size, sizeof(int), NULL);
	send(Connection, msg.c_str(), msg_size, NULL);
	return 0;
}

std::string get_msg() {
	int res_size;
	recv(Connection, (char*)&res_size, sizeof(int), NULL);
	char* res = new char[res_size + 1];
	res[res_size] = '\0';
	recv(Connection, res, res_size, NULL);
	std::string result(res);
	delete[] res;
	return result;
}

int user_creating() {
	CSimpleIniA ini;
	ini.SetUnicode();
	if (ini.LoadFile("user.ini") == SI_FILE) {
		std::cerr << "Ошибка при загрузке INI файла." << std::endl;
		return -1;
	}

	const char* section = "User";
	const char* key;

	std::cout << get_msg() << std::endl;
	std::string msg1;
	std::cout << get_msg() << std::endl;
	std::cout << get_msg() << std::endl;
	std::cout << get_msg() << std::endl;

	msg1 = get_msg();
	key = "phone_number";
	const char* phone_number = msg1.c_str();
	ini.SetValue(section, key, phone_number);
	msg1 = get_msg();
	const char* FIO = msg1.c_str();
	key = "FIO";
	ini.SetValue(section, key, phone_number);
	ini.SaveFile("user.ini");

	return 0;
}

int load_data() {
	CSimpleIniA ini;
	ini.SetUnicode();
	if (ini.LoadFile("user.ini") == SI_FILE) {
		std::cerr << "Ошибка при загрузке INI файла." << std::endl;
		return -1;
	}
	
	const char* section = "user";
	const char* key;
	const char* value;
	get_msg();
	get_msg();
	key = "phone_number";
	value = ini.GetValue(section, key, nullptr);
	send_msg(value);
	return 0;
}

int user_authorization() {
	CSimpleIniA ini;
	ini.SetUnicode();
	if (ini.LoadFile("user.ini") == SI_FILE) {
		std::cerr << "Ошибка при загрузке INI файла." << std::endl;
		return -1;
	}

	CSimpleIniA::TNamesDepend sections;
	ini.GetAllSections(sections);
	
	if (sections.empty() == false) {
		load_data();
	}
	std::cout << get_msg() << std::endl;
}

void DoCommand(char* msg) {
	if (strcmp(msg, "user_creating") == 0) {
		user_creating();
	}
	if (strcmp(msg, "authorization") == 0) {
		user_authorization();
	}
}

void ClientHandler() {
	int msg_size;
	while (true) {
		recv(Connection, (char*)&msg_size, sizeof(int), NULL);
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(Connection, msg, msg_size, NULL);
		std::cout << msg << std::endl;
		DoCommand(msg);
		delete[] msg;
	}
}

int main(int argc, char* argv[]) {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	WSAStartup;
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

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "Error: failed connect to server.\n";
		return 1;
	}
	std::cout << "Connected!\n";

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

	std::string msg1;
	while (true) {
		std::getline(std::cin, msg1);
		int msg_size = msg1.size();
		send(Connection, (char*)&msg_size, sizeof(int), NULL);
		send(Connection, msg1.c_str(), msg_size, NULL);
		Sleep(10);
	}

	system("pause");
	return 0;
}