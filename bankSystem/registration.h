#ifndef REGISTRATION_H
#define REGISTRATION_H

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
#include <string>
#include <iostream>

#include "SimpleIni.h"

#include "mainwindow.h"
#include "main_widget.h"
#include "connection.h"

class MainWidget;
class MainWindow;
class connection;

class Registration : public QWidget
{
    Q_OBJECT

public:
    friend class MainWindow;

    Registration(QWidget *parent = nullptr, MainWidget* mainWidget = nullptr);
    ~Registration();
    int go_authorization();
    int user_creating();
private:
    QFont font;
    QFont input_font;
    QLabel* title;
    QLineEdit* FIO;
    QLineEdit* phone_number;
    QLineEdit* email;
    QLineEdit* password;

    QLabel* user_label;
    QLabel* phone_label;
    QLabel* email_label;
    QLabel* password_label;
    QLabel* error;

    QHBoxLayout* title_layout;
    QHBoxLayout* user_layout;
    QHBoxLayout* phone_layout;
    QHBoxLayout* email_layout;
    QHBoxLayout* password_layout;
    QHBoxLayout* submint_btn_layout;

    QPushButton* submit_registration;

    MainWindow *mainWindow;

protected:
};

#endif // REGISTRATION_H
