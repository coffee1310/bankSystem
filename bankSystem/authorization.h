#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

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

#include "SimpleIni.h"

#include "main_menu.h"
#include "main_widget.h"
#include "connection.h"

class connection;

class Authorization : public QWidget
{
    Q_OBJECT
public:
    Authorization(QWidget* parent = nullptr, MainWidget* mainWidget = nullptr);
    ~Authorization();
    int user_authorization();
    const char* read_phone_number();

private:
    QFont font;
    QFont input_font;

    QVBoxLayout* Layout;
    QLineEdit* phone_number;
    QLineEdit* password;

    QLabel* title;
    QLabel* phone_label;
    QLabel* password_label;

    QHBoxLayout* title_layout;
    QHBoxLayout* user_layout;
    QHBoxLayout* phone_layout;
    QHBoxLayout* email_layout;
    QHBoxLayout* password_layout;
    QHBoxLayout* submint_btn_layout;

    QPushButton* submit_authorization;

    MainWidget *mainWidget;
};

#endif // AUTHORIZATION_H
