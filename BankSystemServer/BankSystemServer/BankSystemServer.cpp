#define WINSOCK_DEPRECATED_NO_WARNINGS
#define CRT_SECURE_NO_WARNINGS
//#define console

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <sqlite3.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

#include <iomanip>
#include <functional>
#include <random>
#include <ctime>
#include <ios>
#include <future>
#include <chrono>
#include <regex>
#include <iomanip>
#include <algorithm>
#include <map>
#include <thread>

std::string md5(const std::string& input);
class UserData;
std::vector<SOCKET> Connections(100);
int Counter = 0;

struct ThreadData {
	int clientIndex;
};

// Структура для видов чеков пользователей
struct UserCheckType {
	std::string id;
	std::string type;
};

// Структура для чеков пользователей
struct UserCheck {
	std::string id;
	std::string check_number;
	std::string balance;
	UserCheckType type = {};
};

// Структура для информации о пользователях
struct UserInformation {
	std::string id;
	std::string FIO;
	std::string phone_number;
	std::string password;
	std::string hash_password;
	bool is_authorized = false;
};

// Структура для информации о транзациях пользователя
struct UserTransaction
{
	std::string id = "";
	std::string check_id = "";
	std::string action = "";
	std::string transaction_date = "";
	std::string second_person = "";
	std::string money_sent = "";
	std::string money_received = "";
	std::string type = "";

	bool operator < (const UserTransaction& other) const {
		return transaction_date < other.transaction_date;
	}
};

// Класс для управления базой данных
class UserDataBaseHandler {
public:
	static int createTables(const char* dir) {
		sqlite3* DB;
		std::string sql;
		char* err_msg;

		try {
			sql = "CREATE TABLE IF NOT EXISTS Users ( "
				"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
				"PHONE_NUMBER VARCHAR(32) NOT NULL UNIQUE, "
				"EMAIL VARCHAR(128) NOT NULL UNIQUE, "
				"PASSWORD VARCHAR(64) NOT NULL, "
				"FIO TEXT NOT NULL"
				");";

			int exit = 0;
			sqlite3_open(dir, &DB);
			exit = sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, &err_msg);

			if (exit != SQLITE_OK) {
				std::cerr << "Database error" << std::endl;
				sqlite3_free(err_msg);
			}

			sql = "CREATE TABLE IF NOT EXISTS Check_type ("
				"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
				"TYPE VARCHAR(32)"
				");";

			exit = sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, &err_msg);

			if (exit != SQLITE_OK) {
				std::cerr << "Database error" << std::endl;
				sqlite3_free(err_msg);
			}

			sql = "CREATE TABLE IF NOT EXISTS Check_purpose_of_use ("
				"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
				"PURPOSE VARCHAR(64) "
				");";

			exit = sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, &err_msg);

			if (exit != SQLITE_OK) {
				std::cerr << "Database error" << std::endl;
				sqlite3_free(err_msg);
			}

			sql = "CREATE TABLE IF NOT EXISTS Checks ("
				"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
				"CHECK_NUMBER VARCHAR(32) UNIQUE, "
				"USER_ID INTEGER NOT NULL, "
				"BALANCE FLOAT NOT NULL, "
				"CHECK_TYPE_ID INT NOT NULL, "
				"PURPOSE_OF_USE_ID INT NOT NULL, "
				"FOREIGN KEY (USER_ID) REFERENCES Users(ID), "
				"FOREIGN KEY (CHECK_TYPE_ID) REFERENCES CHECK_TYPE(ID), "
				"FOREIGN KEY (PURPOSE_OF_USE_ID) REFERENCES CHECK_PURPOSE_OF_USE(ID)"
				");";
			exit = sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, &err_msg);

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
				"SECOND_PERSON_CHECK INT, "
				"MONEY_SENT DOUBLE DEFAULT 0, "
				"MONEY_RECEIVED DOUBLE DEFAULT 0, "
				"FOREIGN KEY (USER_ID) REFERENCES Users (ID)"
				"FOREIGN KEY (CHECK_ID) REFERENCES Checks (ID)"
				"FOREIGN KEY (SECOND_PERSON_CHECK) REFERENCES Checks (ID)"
				");";

			exit = sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, &err_msg);

			if (exit != SQLITE_OK) {
				std::cerr << "Database error" << std::endl;
				sqlite3_free(err_msg);
			}

			sql = "CREATE TABLE IF NOT EXISTS Credit_type ("
				"ID INTEGER PRIMARY KEY NOT NULL, "
				"TITLE VARCHAR(128) NOT NULL, "
				"CREDIT_INTEREST FLOAT NOT NULL"
				");";

			exit = sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, &err_msg);

			if (exit != SQLITE_OK) {
				std::cerr << "Database error" << std::endl;
				sqlite3_free(err_msg);
			}

			sql = "CREATE TABLE IF NOT EXISTS Credits ("
				"ID INTEGER PRIMARY KEY, "
				"USER_ID INTEGER NOT NULL, "
				"CREDIT_TYPE_ID INT NOT NULL, "
				"CREDIT_DATE DATE NOT NULL, "
				"DATE_OF_CREDIT_END DATE NOT NULL, "
				"INITIAL_FEE FLOAT NOT NULL, "
				"TOTAL_AMOUNT FLOAT NOT NULL, "
				"CURRENT_AMMOUNT FLOAT NOT NULL, "
				"MONTHLY_PAYMENT FLOAT NOT NULL, "
				"FOREIGN KEY (USER_ID) REFERENCES Users (ID), "
				"FOREIGN KEY (CREDIT_TYPE_ID) REFERENCES Credit_type (ID)"
				");";

			exit = sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, &err_msg);

			if (exit != SQLITE_OK) {
				std::cerr << "Database error" << std::endl;
				sqlite3_free(err_msg);
			}
			sqlite3_close(DB);

		}
		catch (std::exception& ex) {
			std::cout << "Database exception " << std::endl;
		}
		return 0;
	}

	static std::string getUserIdByPhone(const char* dir, std::string phone_number) {
		sqlite3* DB;
		char* err_msg;
		std::string sql = "SELECT ID FROM Users WHERE PHONE_NUMBER = " + phone_number;
		std::string user_id;

		sqlite3_open(dir, &DB);
		int exit = sqlite3_exec(DB, sql.c_str(), UserDataBaseHandler::getUserIdCallback, &user_id, nullptr);
		if (exit != SQLITE_OK) {
			std::cerr << "Database error" << std::endl;
			sqlite3_free(err_msg);
		}

		sqlite3_close(DB);
		return user_id;
	}

	static int getUserIdCallback(void* data, int argc, char** argv, char** azColName) {
		auto* id = static_cast<std::string*>(data);
		*id = argv[0];
		return 0;
	}

	static int getUsersChecksCallback(void* data, int argc, char** argv, char** azColName) {
		auto* checks = static_cast<std::vector<UserCheck>*>(data);
		UserCheck check;

		check.id = argv[0];
		check.check_number = argv[1];
		check.balance = argv[3];
		checks->push_back(check);
		return 0;
	}

	static int getUserCheckCallback(void* data, int argc, char** argv, char** azColName) {
		auto* check = static_cast<UserCheck*>(data);

		check->id = argv[0];
		check->check_number = argv[1];
		check->balance = argv[3];
		return 0;
	}

	static std::vector<UserCheck> getUserChecks(const std::string& phone_number, std::vector<UserCheck>& user_checks, const char* dir) {
		sqlite3* DB;
		std::string sql;
		char* err_msg;
		auto* checks = new std::vector<UserCheck>;
		auto* user = new UserInformation;

		user->phone_number = phone_number;
		sql = "SELECT * FROM Checks WHERE USER_ID = (SELECT ID FROM Users WHERE PHONE_NUMBER = '" + phone_number + "')";

		int exit = 0;
		sqlite3_open(dir, &DB);
		exit = sqlite3_exec(DB, sql.c_str(), getUsersChecksCallback, checks, &err_msg);

		if (exit != SQLITE_OK) {
			std::cerr << "Database error" << std::endl;
			sqlite3_free(err_msg);
		}

		sqlite3_close(DB);
		user_checks = *checks;
		return *checks;
	}

	static int addUser(const char* dir, const std::string& phone_number, const std::string& FIO, const std::string& email ,const std::string& password) {
		sqlite3* DB;
		std::string sql = "INSERT INTO Users (PHONE_NUMBER, PASSWORD, FIO, EMAIL) "
			"VALUES ('" + phone_number + "', '" + password + "', '" + FIO + "'" + ", '" + email + "'" + ")";
		char* err_msg;

		sqlite3_open(dir, &DB);
		
		int exit = 0;
		exit = sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, &err_msg);
		if (exit != SQLITE_OK) {
			std::cerr << "Database error" << std::endl;
			sqlite3_free(err_msg);
		}

		sqlite3_close(DB);
		return 0;
	}

	static int UserDataVerificationCallback(void* data, int argc, char** argv, char** azColName) {
		auto* callbackData = static_cast<UserInformation*>(data);
		std::string hash_password = md5(callbackData->password);

		if (strcmp(hash_password.c_str(), argv[3]) == 0) {
			callbackData->is_authorized = true;
			callbackData->FIO = argv[4];
		}
		return 0;
	}

	static int UserDataVerification(const char* dir, UserInformation* callbackData, const std::string& phone_number) {
		sqlite3* DB;
		std::string sql;
		char* err_msg;
		sql = "SELECT * FROM Users WHERE PHONE_NUMBER = '" + phone_number + "' LIMIT 1";
		int exit = 0;

		sqlite3_open(dir, &DB);
		exit = sqlite3_exec(DB, sql.c_str(), UserDataVerificationCallback, callbackData, &err_msg);

		if (exit != SQLITE_OK) {
			std::cerr << "Database error" << std::endl;
			sqlite3_free(err_msg);
		}
		
		sqlite3_close(DB);
		return 0;
	}

	static int openCheck(const char* dir, const std::string& phone_number) {
		sqlite3* DB;
		char* err_msg;
		std::string sql = "INSERT INTO Checks (CHECK_NUMBER, USER_ID, BALANCE) "
			"VALUES ('" + std::to_string(getRandomValue()) + "', (SELECT ID FROM Users WHERE PHONE_NUMBER = '" + phone_number + "' LIMIT 1), '0')";
		
		int exit = 0;
		sqlite3_open(dir, &DB);
		exit = sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, &err_msg);

		if (exit != SQLITE_OK) {
			std::cerr << "Database error" << std::endl;
			sqlite3_free(err_msg);
			return -1;
		}

		sqlite3_close(DB);
		delete err_msg;
		return 0;
	}

	static int transferMoneyByPhoneCallback(void* data, int argc, char** argv, char** azColName) {
		auto* callbackData = static_cast<UserCheck*>(data);

		callbackData->balance = argv[1];
		callbackData->check_number = argv[0];
		return 0;
	}

	static int getUserInfo(void* data, int argc, char** argv, char** azColName) {
		auto* callbackData = static_cast<UserInformation*>(data);

		callbackData->id = argv[0];
		callbackData->phone_number = argv[1];
		callbackData->FIO = argv[2];
		return 0;
	}

	static int transferMoneyByPhone(
		const char* dir, 
		const std::string& sender_phone,
		const UserCheck& check,
		float cash,
		const std::string& recephier_phone
	) {
		if (std::stoi(check.balance) < cash)
			return -1;

		std::string cash_str = std::to_string(cash);
		size_t point_pos = cash_str.find(',');
		if (point_pos != -1) {
			cash_str = cash_str.replace(point_pos, 1, ".");
		}

		sqlite3* DB;
		char* err_msg;
		auto* user_sender = new UserInformation;
		auto* user_recephier = new UserInformation;
		auto* sender_check = new UserCheck;
		int exit = 0;

		try
		{
			std::string sql = "SELECT CHECK_NUMBER, BALANCE FROM Checks "
				"WHERE USER_ID = (SELECT ID FROM Users WHERE PHONE_NUMBER = " + recephier_phone + " LIMIT 1)";
			auto* user_check = new UserCheck;
			sqlite3_open(dir, &DB);
			exit = sqlite3_exec(DB, sql.c_str(), transferMoneyByPhoneCallback, user_check, &err_msg);

			if (exit) {
				std::cerr << "Database error" << std::endl;
				sqlite3_free(err_msg);
			}

			sql = "UPDATE Checks SET BALANCE = BALANCE - '" + cash_str +
				"' WHERE USER_ID = (SELECT ID FROM USERS WHERE PHONE_NUMBER = '" + sender_phone + "')";
			exit = sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, &err_msg);

			if (exit) {
				std::cerr << "Database error" << std::endl;
				sqlite3_free(err_msg);
			}

			sql = "UPDATE Checks SET BALANCE = BALANCE + '" + cash_str +
				"' WHERE USER_ID = (SELECT ID FROM USERS WHERE PHONE_NUMBER = '" + recephier_phone + "')";
			exit = sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, &err_msg);

			if (exit) {
				std::cerr << "Database error" << std::endl;
				sqlite3_free(err_msg);
			}

			sql = "SELECT ID, PHONE_NUMBER , FIO FROM USERS WHERE PHONE_NUMBER = " + sender_phone;
			sqlite3_exec(DB, sql.c_str(), getUserInfo, user_sender, &err_msg);

			sql = "SELECT ID, PHONE_NUMBER , FIO FROM USERS WHERE PHONE_NUMBER = " + recephier_phone;
			sqlite3_exec(DB, sql.c_str(), getUserInfo, user_recephier, &err_msg);

			AddTransaction(dir, user_sender->id, check.id, "transfer", user_sender->FIO, cash_str);
			AddTransaction(dir, user_recephier->id, check.id, "receiving money", user_recephier->FIO, "0", cash_str);
			sqlite3_close(DB);

			delete err_msg;
			delete user_sender;
			delete sender_check;
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
		
		return 0;
	}

	static int transferMoneyByCheck(
		const char* dir, 
		const UserCheck& sender_check,
		const UserCheck& recephier_check,
		const std::string& sender_phone,
		float cash
	) {
		if (std::stoi(sender_check.balance) < cash)
			return -1;

		std::string cash_str = std::to_string(cash);
		size_t point_pos = cash_str.find(',');

		if (point_pos != -1) {
			cash_str = cash_str.replace(point_pos, 1, ".");
		}

		sqlite3* DB;
		char* err_msg;
		auto* user_sender = new UserInformation;
		auto* user_recephier = new UserInformation;
		int exit;

		try
		{
			std::string sql = "UPDATE Checks SET BALANCE = BALANCE + " + cash_str  +
				" WHERE CHECK_NUMBER = '" + recephier_check.check_number + "'";
			auto* user_check = new UserCheck;
			sqlite3_open(dir, &DB);
			exit = sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, &err_msg);

			if (exit) {
				std::cerr << "Database error" << std::endl;
				sqlite3_free(err_msg);
				return -1;
			}

			sql = "UPDATE Checks SET BALANCE = BALANCE - " + cash_str +
				" WHERE ID = " + sender_check.id;
			exit = sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, &err_msg);
			if (exit) {
				std::cerr << "Database error" << std::endl;
				sqlite3_free(err_msg);
				return -1;
			}

			sql = "SELECT ID, PHONE_NUMBER , FIO FROM USERS WHERE PHONE_NUMBER = '" + sender_phone + "'";
			exit = sqlite3_exec(DB, sql.c_str(), getUserInfo, user_sender, &err_msg);

			if (exit) {
				std::cerr << "Database error" << std::endl;
				sqlite3_free(err_msg);
			}

			sql = "SELECT ID, PHONE_NUMBER , FIO FROM USERS "
				"WHERE ID = (SELECT USER_ID FROM Checks WHERE CHECK_NUMBER = '" + recephier_check.check_number + "')";
			sqlite3_exec(DB, sql.c_str(), getUserInfo, user_recephier, &err_msg);

			if (exit) {
				std::cerr << "Database error" << std::endl;
				sqlite3_free(err_msg);
				return -1;
			}

			AddTransaction(dir, user_sender->id, sender_check.id, "transfer", user_recephier->FIO, cash_str);
			AddTransaction(dir, user_recephier->id, recephier_check.id, "receiving money", user_sender->FIO, "0", cash_str);
			sqlite3_close(DB);

			delete err_msg;
			delete user_check;
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}

		return 0;
	}

	static int ApplyForACreditCreditTypeCallback(void* data, int argc, char** argv, char** azColName) {
		auto* callbackData = static_cast<std::string*>(data);
		*callbackData = argv[0];
		return 0;
	}

	static int ApplyForACredit(
		const char* dir,
		const std::string& user_id,
		const std::string& credit_type,
		const std::string& credit_term,
		const std::string& initial_fee,
		const std::string& total_amount
	) {
		std::time_t t = std::time(nullptr);
		char date_now[64];
		strftime(date_now, sizeof(date_now), "%d.%m.%Y", localtime(&t));
		t = std::time(nullptr) + std::stoi(credit_term) * 30 * 24 * 60 * 60;

		char date_of_credit_end[64];
		strftime(date_of_credit_end, sizeof(date_of_credit_end), "%d.%m.%Y", localtime(&t));

		float monthly_payment = (std::atol(total_amount.c_str()) - std::atol(initial_fee.c_str())) / std::stoi(credit_term);
		std::string monthly_payment_str = std::to_string(monthly_payment);
		size_t point_pos = monthly_payment_str.find(',');
		if (monthly_payment_str.find(',') != -1) {
			monthly_payment_str = monthly_payment_str.replace(point_pos, 1, ".");
		}

		sqlite3* DB;
		auto* credit_id = new std::string;
		std::string sql = "SELECT ID FROM Credit_type WHERE TITLE = '" + credit_type + "'";
		sqlite3_open(dir, &DB);
		char* err_msg;

		int rc = sqlite3_exec(DB, "PRAGMA encoding = 'UTF-8';", nullptr, nullptr, 0);
		if (rc != SQLITE_OK) {
			std::cerr << "Failed to set encoding: " << sqlite3_errmsg(DB) << std::endl;
		}

		int exit = sqlite3_exec(DB, sql.c_str(), ApplyForACreditCreditTypeCallback, credit_id, &err_msg);
		
		sql = "INSERT INTO Credits (USER_ID, CREDIT_DATE, CREDIT_TYPE_ID, "
			"DATE_OF_CREDIT_END, INITIAL_FEE, TOTAL_AMOUNT, CURRENT_AMMOUNT, MONTHLY_PAYMENT) "
			"VALUES ('" + user_id + "', '" + date_now + "'," + *credit_id + ", '"
			+ date_of_credit_end + "', '" + initial_fee + "', '" + total_amount + "', '" + initial_fee + "', "
			"(SELECT CREDIT_INTEREST FROM Credit_type WHERE TITLE = '" + credit_type + "') * " + monthly_payment_str + ")";
		
		if (exit != SQLITE_OK) {
			std::cerr << "SQL error: " << err_msg << std::endl;
			const void* errMsg = sqlite3_errmsg16(DB);
			if (errMsg != nullptr) {
				wprintf(L"Ошибка при открытии базы данных: %ls\n", errMsg);
			}
			else {
				wprintf(L"Произошла непредвиденная ошибка при открытии базы данных\n");
			}

			sqlite3_free(err_msg);
			sqlite3_close(DB);
			return 1;
		}

		sqlite3_close(DB);
		return 0;
	}

	static int AddTransaction(
		const char* dir,
		const std::string& user_id,
		const std::string& check_id,
		const std::string& action,
		const std::string& second_person="NULL",
		const std::string& money_sent="0",
		const std::string& money_received="0"
	) {
		std::time_t t = std::time(nullptr);
		char time_str[64];
		strftime(time_str, sizeof(time_str), "%d.%m.%Y", localtime(&t));
		int exit;
		sqlite3* DB;
		
		std::string sql = "INSERT INTO Transactions (USER_ID, CHECK_ID, ACTION, TRANSACTION_DATE, SECOND_PERSON, MONEY_SENT, MONEY_RECEIVED) "
			"VALUES(" + user_id + ", " + check_id + ", '" + action + "', '" + time_str + "', " + second_person + ", " + money_sent + ", " + money_received + ")";
		char* err_msg;

		sqlite3_open(dir, &DB);
		exit = sqlite3_exec(DB, sql.c_str(), nullptr, NULL, &err_msg);

		if (exit) {
			std::cerr << "Database error" << std::endl;
			sqlite3_free(err_msg);
			return -1;
		}

		sqlite3_close(DB);
		delete err_msg;

		return 0;
	}

	static std::vector<UserTransaction> getTransactions(const char* dir, const std::string& user_id, const std::string& period="all") {
		sqlite3* DB;
		std::string sql;

		if (period == "all") {
			sql = "SELECT * FROM Transactions WHERE USER_ID = " + user_id;
		}
		else if (period == "year") {
			sql = "SELECT * FROM Transactions WHERE strftime('%Y', TRANSACTION_DATE) = strftime('%Y', 'now') AND USER_ID = "
			+ user_id + ";";
		}
		else if (period == "half_year") {
			sql = "SELECT * FROM Transactions WHERE TRANSACTION_DATE >= date('now', '-6 months') AND USER_ID = "
			+ user_id + ";";
		}
		else if (period == "month") {
			sql = "SELECT * FROM Transactions WHERE TRANSACTION_DATE >= date('now', '-1 month') AND USER_ID = "
			+ user_id + ";";
		}

		char* err_msg;
		sqlite3_open(dir, &DB);

		auto* transactions = new std::vector<UserTransaction>;
		int exit = sqlite3_exec(DB, sql.c_str(), getTransactionCallback, transactions, &err_msg);

		if (exit != SQLITE_OK) {
			std::cerr << "Database error" << std::endl;
			sqlite3_free(err_msg);
		}

		sqlite3_close(DB);
		return *transactions;
	}

	static std::map<std::string, float> getCostTransactionsTypeSum(const char* dir, const std::string& user_id, const std::string& period="all") {
		sqlite3* DB;
		std::string sql = "SELECT * FROM Check_type;";
		std::map<std::string, float> cost_type;
		std::vector<UserCheckType> check_types = getCheckTypes(dir);

		char* err_msg;
		sqlite3_open(dir, &DB);

		int sum = 0;

		for (const UserCheckType& check_type: check_types) {
			sql = "SELECT SUM(DISTINCT TRANSACTIONS.MONEY_SENT) FROM TRANSACTIONS "
			"JOIN CHECKS ON TRANSACTIONS.USER_ID = " + user_id + "AND TRANSACTIONS.MONEY_SENT > 0 AND TRANSACTIONS.MONEY_RECEIVED = 0 WHERE CHECKS.PURPOSE_OF_USE_ID = "  + check_type.id;

			sqlite3_exec(DB, sql.c_str(), getCostTransactionsTypeSumCallback, &sum, &err_msg);
			cost_type[check_type.type] = sum;
		}

		return cost_type;
	}
private:
	// Функция для получения случайного номера чека
	static long getRandomValue() {
		std::random_device rd;
		std::mt19937 gen(rd());
		long long min = 100000000000000LL;
		long long max = 999999999999999LL;
		std::uniform_int_distribution<long long> dist(min, max);
		int random_number = dist(gen);
		return random_number;
	}

	static int getTransactionCallback(void* data, int argc, char** argv, char** azColName) {
		auto* transactions = static_cast<std::vector<UserTransaction>*>(data);
		UserTransaction transaction{};
		transaction.id = argv[0];
		transaction.check_id = argv[2];
		transaction.action = argv[3];
		transaction.transaction_date = argv[4];
		transaction.second_person = argv[5];
		transaction.money_sent = argv[6];
		transaction.money_received = argv[7];

		transactions->push_back(transaction);
		return 0;
	}

	static std::vector<UserCheckType> getCheckTypes(const char* dir) {
		sqlite3* DB;
		std::string sql = "SELECT * FROM Check_type;";
		char* err_msg;
		auto* check_types = new std::vector<UserCheckType>;

		sqlite3_open(dir, &DB);
		sqlite3_exec(DB, sql.c_str(), getCheckTypesCallback, check_types, &err_msg);

		return *check_types;
	}

	static int getCheckTypesCallback(void* data, int argc, char** argv, char** azColName) {
		auto* types = static_cast<std::vector<UserCheckType>*>(data);
		UserCheckType type;
		type.id = argv[0];
		type.type = argv[1];

		types->push_back(type);
		return 0;
	}

	static int getCostTransactionsTypeSumCallback(void* data, int argc, char** argv, char** azColName) {
		int* count = static_cast<int*>(data);
		*count = std::stoi(argv[0]);
		return *count;
	}
};

class UserData {
public:
	explicit UserData(SOCKET Connection) {
		this->Connection = Connection;
		password = "none";
		FIO = "none";
		phone_number = "none";
	}
	
	void setAction(const std::string& action) {
		if (action == "create_user") {
			this->createUser();
		}
		else if (action == "login") {
			this->loginUser();
		}
		else if (!this->is_authorized) {
			send_msg("Авторизируйетсь");
		}
		else if (action == "get_checks") {
			this->getUserChecks();
		}
		else if (action == "open_check") {
			this->openCheck();
		}
		else if (action == "transfer_money_by_phone") {
			this->transferMoneyByPhone();
		}
		else if (action == "transfer_money_by_check") {
			this->transferMoneyByCheck();
		}
		else if (action == "apply_for_a_credit") {
			this->ApplyForACredit();
		}
		else if (action == "send_transactions") {
			this->sendTransactions();
		} else if (action == "send_transaction_types") {
			this->sendTransactionsType();
		}
	}
	
	UserData(const UserData& copy) {
		Connection = copy.Connection;
		password = copy.password;
		FIO = copy.FIO;
		phone_number = copy.phone_number;
	}
private:
	const char* dir = R"(Bank.db)";
	std::string phone_number;
	std::string password;
	std::string FIO;
	std::string email;
	std::vector<UserCheck> user_checks;
	bool is_authorized = false;
	SOCKET Connection;

	int send_msg(const std::string& msg) const {
		int msg_size = msg.size();
		int send_bytes = send(this->Connection, (char*)&msg_size, sizeof(int), 0);

		//std::cout << "1: " << msg << std::endl;
		//std::cout << "1" << send_bytes << std::endl;

		if (send_bytes != sizeof(int)) {
			return -1;
		}

		send_bytes = send(this->Connection, msg.c_str(), msg_size, 0);

		if (send_bytes != msg_size) {
			return -1;
		}

		//std::cout << "2: " << msg << std::endl;
		//std::cout << "2" << send_bytes << std::endl;
		return 0;
	}

	std::string get_msg() const {
		int res_size = 0;
		int received_bytes = recv(this->Connection, (char*)&res_size, sizeof(int), 0);

		if (received_bytes <= 0) {
			return "";
		}

		if (res_size <= 0) {
			return "";
		}

		char* res = new char[res_size + 1];
		res[res_size] = '\0';

		received_bytes = recv(this->Connection, res, res_size, 0);
		if (received_bytes != res_size) {
			delete[] res;
			return "";
		}

		std::string result(res);
		delete[] res;
		return result;
	}

	int createTables() {
		UserDataBaseHandler::createTables(this->dir);
		return 0;
	}

	int createUser() {
		std::string password_;
		this->createTables();

		#ifdef console
			this->send_msg("Введите ФИО: ");
		#endif

		this->FIO = get_msg();

		#ifdef console
			this->send_msg("Введите номер телефона: ");
		#endif

		this->phone_number = get_msg();

		#ifdef console
			this->send_msg("Введите email: ");
		#endif

		this->email = get_msg();

		#ifdef console
			this->send_msg("Введите пароль: ");
		#endif	

		password_ = get_msg();
		this->password = md5(password_);

		if (!CheckEmail(this->email)) {
			send_msg("error email");
			return 1;
		}
		if (!CheckNumber(this->phone_number)) {
			send_msg("error phone");
			return 1;
		}
		if (!CheckFIO(this->FIO)) {
			send_msg("error FIO");
			return 1;
		}

		UserDataBaseHandler::addUser(this->dir, this->phone_number, this->FIO, this->email, this->password);

		send_msg("user created");

		#ifdef console
			send_msg(this->phone_number);
			send_msg(this->FIO);
		#endif
		
		return 0;
	}

	int UserDataVerification() {
		auto* callbackData = new UserInformation;
		callbackData->phone_number = this->phone_number;
		callbackData->password = this->password;
		UserDataBaseHandler::UserDataVerification(this->dir, callbackData, this->phone_number);

		this->is_authorized = callbackData->is_authorized;
		this->FIO = callbackData->FIO;

		delete callbackData;
		return 0;
	}

	int loginUser() {
		#ifdef console
			send_msg("authorization");
			send_msg("Загрузка данных");
		#endif

		this->phone_number = get_msg();

		#ifdef console
			send_msg("Введите пароль");
		#endif

		this->password = get_msg();
		this->UserDataVerification();

		if (this->is_authorized) {
			send_msg("you are logged in");
			#ifdef console
				this->getUserChecks();
			#endif
		}
		else
		{
			send_msg("you are not logged in");
			this->password = "";
			this->phone_number = "";
		}

		return 0;
	}

	int getUserChecks() {
		this->user_checks = UserDataBaseHandler::getUserChecks(this->phone_number, this->user_checks, this->dir);
		std::string checks_msg;
		send_msg(std::to_string(this->user_checks.size()));

		for (auto & user_check : this->user_checks) {
			checks_msg = user_check.check_number;
			send_msg(checks_msg);
			checks_msg = user_check.balance;
			send_msg(checks_msg);
		}
		
		return 0;
	}

	int openCheck() {
		if (!UserDataBaseHandler::openCheck(this->dir, this->phone_number)) {
			send_msg("Счет создан");
		}
		else {
			send_msg("Не удалось открыть счет");
		}

		return 0;
	}

	int transferMoneyByPhone() {
		std::string recipient_number;
		std::string check_number;
		std::string cash;
		UserCheck check;

		#ifdef console
			send_msg("Введите номер телефона получателя");
		#endif
		recipient_number = get_msg();
		#ifdef console
			send_msg("Введите ваш номер счета");
		#endif
			check_number = get_msg();
		#ifdef console
			send_msg("Введите сумму");
		#endif

		cash = get_msg();
		check.check_number = "";

		for (auto & user_check : this->user_checks) {
			if (strcmp(user_check.check_number.c_str(), check_number.c_str()) != -1) {
				check = user_check;
			}
		}

		int callback = 0;
		if (check.check_number != "")
			 callback = UserDataBaseHandler::transferMoneyByPhone(this->dir,this->phone_number, check, std::stoi(cash), recipient_number);
		if (callback == 0) {
			send_msg("Перевод осуществлен");
			return 0;
		}
		else {
			send_msg("Перевод не осуществлен");
			return -1;
		}
	}

	int transferMoneyByCheck() {
		std::string recipient_check_str;
		std::string sender_check_str;
		std::string cash;
		UserCheck sender_check;
		auto* recipient_check = new UserCheck;

		#ifdef console
			send_msg("Введите номер счета получателя");
		#endif

		recipient_check_str = get_msg();

		#ifdef console
			send_msg("Введите ваш номер счета");
		#endif

		sender_check_str = get_msg();

		#ifdef console
			send_msg("Введите сумму");
		#endif

		cash = get_msg();

		sqlite3* DB;
		int exec;
		std::string sql = "SELECT * FROM Checks "
			"WHERE CHECK_NUMBER = '" + recipient_check_str + "'";
		char* err_msg;
		sqlite3_open(this->dir, &DB);
		exec = sqlite3_exec(DB, sql.c_str(), UserDataBaseHandler::getUserCheckCallback, recipient_check, &err_msg);
		if (exec)
			return 0;

		sender_check.check_number = "";
		for (int i = 0; i < this->user_checks.size(); i++) {
			if (strcmp(this->user_checks[i].check_number.c_str(), sender_check_str.c_str()) != -1) {
				sender_check = this->user_checks[i];
			}
		}

		int callback = 0;
		if (!sender_check.check_number.empty())
			callback = UserDataBaseHandler::transferMoneyByCheck(this->dir, sender_check, *recipient_check, this->phone_number, std::stoi(cash));
		
		delete recipient_check;
		if (callback == 0) {
			send_msg("Перевод осуществлен");
			return 0;
		}
		else {
			send_msg("Перевод не осуществлен");
			return -1;
		}
	}

	int ApplyForACredit(){
		std::string credit_type;
		std::string credit_term;
		std::string initial_fee;
		std::string total_amount;

		#ifdef console
			send_msg("Выберите вид кредита");
		#endif

		credit_type = get_msg();

		#ifdef console
			send_msg("Выберите срок");
		#endif

			credit_term = get_msg();

		#ifdef console
			send_msg("Введите размер кредита");
		#endif

		total_amount = get_msg();

		#ifdef console
			send_msg("Введи первоначальный взнос");
		#endif

		initial_fee = get_msg();

		std::string user_id;

		user_id = UserDataBaseHandler::getUserIdByPhone(this->dir, this->phone_number);
		UserDataBaseHandler::ApplyForACredit(this->dir, user_id, credit_type, credit_term, initial_fee, total_amount);

		return 0;
	}

	// void getCreditPayment() {
	// 	std::cout << 1 << std::endl;
	// 	std::this_thread::sleep_for(std::chrono::seconds(10));
	// 	std::cout << 2 << std::endl;
	// }

	void sendTransactions() {
		std::string user_id;
		std::string period;
		std::vector<UserTransaction> user_transactions;
		std::vector<UserTransaction> cost;
		std::vector<UserTransaction> income;
		std::map <std::string, std::map<int, double>> cost_values;
		std::map <std::string, std::map<int, double>> income_values;

		period = get_msg();
		user_id = UserDataBaseHandler::getUserIdByPhone(this->dir, this->phone_number);
		user_transactions = UserDataBaseHandler::getTransactions(dir, user_id, period);

		cost_values["year"], income_values["year"] = {
			{1, 0},
			{2, 0},
			{3, 0},
			{4, 0},
			{5, 0},
			{6, 0},
			{7, 0},
			{8, 0},
			{9, 0},
			{10, 0},
			{11, 0},
			{12, 0} 
		};

		cost = SortCostTransactions(user_transactions, period, cost_values);
		income = SortIncomeTransactions(user_transactions, period, income_values);

		sendCost(cost);
		sendIncome(income);
	}

	std::vector<UserTransaction> SortCostTransactions(
		std::vector<UserTransaction>& user_transactions,
		const std::string& period,
		std::map <std::string, std::map<int, double>>& cost_values
		) {

		std::vector<UserTransaction> cost;

		for (const UserTransaction& t : user_transactions) {
			if (t.money_sent == "0.0") {
				continue;
			}
			int day, month, year;
			if (sscanf(t.transaction_date.c_str(), "%d-%d-%d", &year, &month, &day) == 3)
			{
				printf("Date is %02d.%02d.%d\n", year, month, day + 2000);
			}
			if (period == "year") {
				cost_values[period][month] += std::stod(t.money_sent);
			}
			cost.push_back(t);
		}

		std::sort(cost.begin(), cost.end());
		return cost;
	}

	std::vector<UserTransaction> SortIncomeTransactions(
		std::vector<UserTransaction>& user_transactions,
		const std::string& period,
		std::map <std::string, std::map<int, double>>& income_values) {

		std::vector<UserTransaction> income;
		for (const UserTransaction& t : user_transactions) {
			if (t.money_received == "0.0") {
				continue;
			}
			int day, month, year;
			if (sscanf(t.transaction_date.c_str(), "%d-%d-%d", &year, &month, &day) == 3)
			{
				printf("Date is %02d.%02d.%d\n", year, month, day + 2000);
			}
			if (period == "year") {
				income_values[period][month] += std::stod(t.money_received);
			}
			income.push_back(t);
		}

		std::sort(income.begin(), income.end());
		return income;
	}

	int sendCost(const std::vector<UserTransaction>& cost) {
		send_msg("Cost");
		send_msg(std::to_string(cost.size()));
		for (const UserTransaction& t : cost) {
			send_msg(t.transaction_date);
			send_msg(t.money_sent);
		}

		return 0;
	}

	int sendIncome(const std::vector<UserTransaction>& income) {
		send_msg("Income");
		send_msg(std::to_string(income.size()));
		for (const UserTransaction& t : income) {
			send_msg(t.transaction_date);
			send_msg(t.money_received);
		}

		return 0;
	}

	int sendTransactionsType() {
		std::string period = get_msg();
		std::string user_id;
		user_id = UserDataBaseHandler::getUserIdByPhone(this->dir, this->phone_number);

		std::map<std::string, float> transactions_type = UserDataBaseHandler::getCostTransactionsTypeSum(dir, user_id, "year");
		send_msg(std::to_string(transactions_type.size()));

		for (const auto& t : transactions_type) {
			send_msg(t.first);
			send_msg(std::to_string(t.second));
		}
		return 0;
	}

	static bool CheckEmail(const std::string& str)
	{
		std::regex rx("^[A-Za-z0-9]+([A-Za-z0-9._-]+)*[A-Za-z0-9]@[A-Za-z0-9]+\\.[A-Za-z]{2,4}$");
		return std::regex_match(str, rx);;
	}

	static bool CheckNumber(const std::string& str) {
		std::regex rx(R"((^8\d{10})|(^\+7\d{10}))");
		return  std::regex_match(str, rx);;
	}

	static bool CheckFIO(const std::string& str) {
		std::regex pattern("^[^0-9!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~]+$");
		return std::regex_match(str, pattern);
	}
};

std::string md5(const std::string& input) {
	EVP_MD_CTX* ctx = EVP_MD_CTX_new();
	if (!ctx) {
		std::cerr << "Error creating context" << std::endl;
		return "";
	}

	if (1 != EVP_DigestInit_ex(ctx, EVP_md5(), nullptr)) {
		std::cerr << "Error initializing MD5" << std::endl;
		EVP_MD_CTX_free(ctx);
		return "";
	}

	if (1 != EVP_DigestUpdate(ctx, input.c_str(), input.size())) {
		std::cerr << "Error updating MD5" << std::endl;
		EVP_MD_CTX_free(ctx);
		return "";
	}

	unsigned char digest[EVP_MAX_MD_SIZE];
	unsigned int digestLength;
	if (1 != EVP_DigestFinal_ex(ctx, digest, &digestLength)) {
		std::cerr << "Error finalizing MD5" << std::endl;
		EVP_MD_CTX_free(ctx);
		return "";
	}

	std::stringstream ss;
	for (unsigned int i = 0; i < digestLength; ++i) {
		ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
	}

	EVP_MD_CTX_free(ctx);
	return ss.str();
	return "";
}

std::vector<UserData*> users(100);

std::string do_command(const std::string& msg, SOCKET connection, int index)
{
	UserData* user = (users[index]);
	user->setAction(msg);
	users[index] = user;
	
	return "";
}

void ClientHandler(LPVOID lpParam) {
	auto* data = (ThreadData*)lpParam;
	int index = data->clientIndex;
    int msg_size;
	SOCKET clientSocket = Connections[index];
	UserData* userData = users[index];

    while (true) {
        int recv_result = recv(clientSocket, (char*)&msg_size, sizeof(int), 0);
		if (recv_result <= 0) {
			std::cout << "Клиент отключился" << std::endl;
			closesocket(clientSocket);
			delete userData;
			Connections.erase(Connections.begin() + index);
			users.erase(users.begin() + index);
			return;
		}

        char* msg = new char[msg_size + 1];
        msg[msg_size] = '\0';
        recv_result = recv(Connections[index], msg, msg_size, 0);

		if (recv_result < 0) {
			std::cout << "Клиент отключился" << std::endl;
			closesocket(clientSocket);
			delete userData;
			Connections.erase(Connections.begin() + index);
			users.erase(users.begin() + index);
			return;
		}

		std::cout << msg << std::endl;
        for (int i = 0; i < Counter; i++) {
			if (i == index) {
 				do_command(msg, Connections[i], index);

				#ifdef console
					send(Connections[i], (char*)&result_size, sizeof(int), NULL);
					send(Connections[i], result.c_str(), result_size, NULL);
				#endif
				continue;
			}
        }
        delete[] msg;
    }
}

int main(int argc, char* argv[]) {
	//std::locale::global(std::locale("en_US.UTF-8"));
	std::setlocale(LC_ALL, "ru_RU.utf8");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	WSAData wsaData{};
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

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	for (int i = 0; i < 100; i++) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

		if (newConnection == 0) {
			std::cout << "Error #2\n";
		}
		else {
			#ifdef console
				std::cout << "Client Connected!\n";
				std::string msg = "Hello. It`s my first network program!";
				int msg_size = msg.size();
				send(newConnection, (char*)&msg_size, sizeof(int), NULL);
				send(newConnection, msg.c_str(), msg_size, NULL);
			#endif	

			Connections[i] = newConnection;
			users[i] = new UserData(newConnection);
			Counter++;
			auto* data = new ThreadData();
			data->clientIndex = i;
			CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)data, 0, 0);
		}
	}

	system("pause");
	return 0;
}