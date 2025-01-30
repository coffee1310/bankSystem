#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <QMainWindow>
#include <QTextEdit>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QEvent>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <QVBoxLayout>
#include <QTcpSocket>
#include <thread>
#include <future>
#include <chrono>
#include <vector>

#include "SimpleIni.h"

#include "main_menu.h"
#include "main_widget.h"
#include "connection.h"
#include "graphic.h"
#include "histogram.h"

class MainWidget;

class MainMenu : public QWidget
{
    Q_OBJECT
public:
    MainMenu(QWidget *parent = nullptr, MainWidget* mainWidget = nullptr);
    ~MainMenu();

private:
    QVBoxLayout* Layout;

    QHBoxLayout* FIO_Messages;
    QLabel* FIO;
    QPushButton* messages;
    QVBoxLayout* operations;
    QVBoxLayout* payments;
    QHBoxLayout* operations_and_payments;
    QVBoxLayout* cards;
    QHBoxLayout* card;
    QVBoxLayout* card_information;
    QPushButton* open_check_btn;
    QHBoxLayout* graphics;

    MainWidget* mainWidget;
    std::vector<Transaction> cost;
    std::vector<Transaction> income;
    std::map<std::string, double> transaction_types;

    int get_transactions(std::string period="year");
    int get_transaction_types(std::string period="year");
};

#endif // MAIN_MENU_H
