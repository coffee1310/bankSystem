#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

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

#include "registration.h"
#include "main_menu.h"
#include "mainwindow.h"
#include "connection.h"
#include "authorization.h"

class Registration;
class Authorization;
class MainMenu;
class MainWindow;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    void switch_to_login();
    void switch_to_registration();
    void switch_to_main_menu();

    const char* read_FIO();
private:
    QVBoxLayout* Layout;
    QHBoxLayout* Layout2;
    Registration* registration;
    Authorization* authorization;
    MainMenu* mainMenu;
    QLabel* login_label;
    QLabel* registration_label;

    QWidget *menu_widget;
    QVBoxLayout* menu;
    QVBoxLayout* user_layout;
    QPushButton* user_btn;
    QIcon user_icon;

    QLabel* main_label;
    QVBoxLayout* transaction_layout;
    QPushButton* transaction_btn;
    QIcon* transaction_icon;

    QLabel* history_label;
    QVBoxLayout* card_layout;
    QPushButton* card_btn;
    QIcon* card_icon;

    QLabel* payments_label;
    QVBoxLayout* chat_layout;
    QPushButton* chat_btn;
    QIcon* chat_icon;

    QLabel* chat_label;

    MainWindow *mainWidget;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override {
        if (obj == login_label) {
            if (event->type() == QEvent::MouseButtonPress) {
                switch_to_login();
                return true;
            }
        }
        if (obj == registration_label) {
            if (event->type() == QEvent::MouseButtonPress) {
                switch_to_registration();
                return true;
            }
        }
        return QWidget::eventFilter(obj, event);
    }
};
#endif // MAIN_WIDGET_H
