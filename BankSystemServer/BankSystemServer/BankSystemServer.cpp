#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <sqlite3.h>
#pragma comment(lib,"Ws2_32.lib")

SOCKET Connections[100];
int Counter = 0;

class UserDataBase {
public:
	UserDataBase(SOCKET Connection) {
		this->createDB();
		this->createTables();
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
	const char* dir = R"(Bank.db)";
	std::string phone_number;
	std::string password;
	std::string FIO;
	SOCKET Connection;
	
	int send_msg(std::string msg) {
		int msg_size;
		send(this->Connection, (char*)&msg_size, sizeof(int), NULL);
		send(this->Connection, msg.c_str(), msg_size, NULL);
		return 0;
	}

	char* get_msg() {
		char* res;
		int res_size;
		std::string msg;
		int msg_size;
		recv(Connection, (char*)&res_size, sizeof(int), NULL);
		res = new char[res_size + 1];
		msg[msg_size] = '\0';
		recv(Connection, res, res_size, NULL);
	}
	
	int createDB() {
		sqlite3* DB;
		int exit = 0;
		exit = sqlite3_open(this->dir, &DB);
		sqlite3_close(DB);

		return 0;
	}

	int createTables() {
		sqlite3* DB;
		std::string sql;
		char* err_msg;

		sql = "CREATE TABLE IF NOT EXISTS Users ( "
			"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
			"PHONE_NUMBER VARCHAR(32) NOT NULL, "
			"PASSWORD VARCHAR(8) NOT NULL, "
			"FIO TEXT NOT NULL"
			");";

		try {
			int exit = 0;
			exit = sqlite3_open(this->dir, &DB);
			exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &err_msg);

			if (exit != SQLITE_OK) {
				std::cerr << "Database error" << std::endl;
				sqlite3_free(err_msg);
			}

			sql = "CREATE TABLE IF NOT EXISTS Checks ("
				"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
				"USER_ID INTEGER NOT NULL, "
				"BALANCE FLOAT NOT NULL, "
				"FOREIGN KEY (USER_ID) REFERENCES Users (ID)"
				");";
			exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &err_msg);

			if (exit != SQLITE_OK) {
				std::cerr << "Database error" << std::endl;
				sqlite3_free(err_msg);
			}

			sql = "CREATE TABLE IF NOT EXISTS Transactions ("
				"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
				"USER_ID INTEGER NOT NULL, "
				"CHECK_ID INT NOT NULL, "
				"ACTION VARCHAR(256) NOT NULL, "
				"TRANSACTION_DATE DATE NOT NULL, "
				"FOREIGN KEY (USER_ID) REFERENCES Users (ID)"
				"FOREIGN KEY (CHECK_ID) REFERENCES Checks (ID)"
				");";

			exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &err_msg);

			if (exit != SQLITE_OK) {
				std::cerr << "Database error" << std::endl;
				sqlite3_free(err_msg);
			}

			sql = "CREATE TABLE IF NOT EXISTS Credits ("
				"ID INTEGER PRIMARY KEY, "
				"USER_ID INTEGER NOT NULL, "
				"CREDIT_DATE DATE NOT NULL, "
				"DATE_OF_CREDIT_END DATE NOT NULL, "
				"TOTAL_AMOUNT FLOAT NOT NULL, "
				"CURRENT_AMMOUNT FLOAT NOT NULL, "
				"CREDIT_INTEREST FLOAT NOT NULL, "
				"MONTHLY_PAYMENT FLOAT NOT NULL, "
				"FOREIGN KEY (USER_ID) REFERENCES Users (ID)"
				");";

			exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &err_msg);

			if (exit != SQLITE_OK) {
				std::cerr << "Database error" << std::endl;
				sqlite3_free(err_msg);
			}

			sqlite3_close(DB);
			delete err_msg;
		}
		catch (const std::exception& e) {
			std::cerr << e.what();
		}

		return 0;
	}

	int addUser() {
		sqlite3* DB;
		std::string sql = "INSERT INTO Users (PHONE_NUMBER, PASSWORD, FIO) "
			"VALUES (" + this->phone_number + ", " + this->password + ", " + this->FIO + ")";
		char* err_msg;

		sqlite3_open(this->dir, &DB);
		try
		{
			int exit = 0;
			exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &err_msg);
			if (exit != SQLITE_OK) {
				std::cerr << "Database error" << std::endl;
				sqlite3_free(err_msg);
			}

			delete err_msg;
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what();
		}
		return 0;
	}

	bool checkPhoneNumber()
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