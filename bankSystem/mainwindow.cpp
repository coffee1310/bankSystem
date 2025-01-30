#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QPainter>
#include <QStyle>
#include <SimpleIni.h>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <QTcpSocket>

SOCKET Connection;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWidget* widget = new MainWidget(this);
    widget->setStyleSheet("background-color: #1A1F25");
    setCentralWidget(widget);
    this->resize(500, 700);
}

MainWindow::~MainWindow()
{
    delete ui;
}


