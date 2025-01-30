#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

#include "main_widget.h"
#include "connection.h"

extern SOCKET Connection;

QT_BEGIN_NAMESPACE

class MainWidget;

namespace Ui {
class MainWindow;
class MainWidget;
class MainMenu;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    friend class Registration;
    friend class Authorization;
    friend class MainMenu;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;


private slots:


};

#endif // MAINWINDOW_H
