#include "main_widget.h"
#include <iostream>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent) {
    Layout = new QVBoxLayout(this);
    registration = new Registration(this, this);
    registration->setStyleSheet("background-color: #1A1F25");
    login_label = new QLabel;
    login_label->installEventFilter(this);
    login_label->setText("Авторизоваться");
    login_label->setAlignment(Qt::AlignCenter);
    login_label->setStyleSheet("color: blue;");
    login_label->setCursor(Qt::PointingHandCursor);

    Layout->addWidget(registration);
    Layout->addWidget(login_label);
    Layout->setAlignment(Qt::AlignCenter);
}

MainWidget::~MainWidget()
{
    delete Layout;
}

const char* MainWidget::read_FIO() {
    CSimpleIniA ini;
    ini.SetUnicode();
    if (ini.LoadFile("user.ini") == SI_FILE) {
        std::cerr << "Ошибка при загрузке INI файла." << std::endl;
        return "";
    }

    const char* section = "user";
    const char* key;
    const char* value;
    key = "FIO";
    value = ini.GetValue(section, key, "");
    return value;
}

void MainWidget::switch_to_login()
{
    QLayoutItem *item;
    while ((item = this->Layout->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }

    authorization = new Authorization(this, this);
    authorization->setStyleSheet("background-color: #1A1F25");
    registration_label = new QLabel(this);
    registration_label->installEventFilter(this);
    registration_label->setText("Зарегистрироваться");
    registration_label->setAlignment(Qt::AlignCenter);
    registration_label->setStyleSheet("color: #5D5D5D;");
    registration_label->setCursor(Qt::PointingHandCursor);

    Layout->addWidget(authorization);
    Layout->addWidget(registration_label);
}

void MainWidget::switch_to_registration()
{
    QLayoutItem *item;
    while ((item = this->Layout->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }

    registration = new Registration(this, this);
    login_label = new QLabel(this);
    login_label->installEventFilter(this);
    login_label->setText("Авторизоваться");
    login_label->setAlignment(Qt::AlignCenter);
    login_label->setStyleSheet("color: #5D5D5D;");
    login_label->setCursor(Qt::PointingHandCursor);
    Layout->addWidget(registration);
    Layout->addWidget(login_label);
}

void MainWidget::switch_to_main_menu()
{
    Layout2 = new QHBoxLayout(this);
    Layout2->setAlignment(Qt::AlignLeft);

    QLayoutItem *item;
    while ((item = this->Layout->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }

    QFont font("Roboto Slab");
    menu_widget = new QWidget(this);
    menu_widget->setStyleSheet("background-color: #10101C;"
                               "width: 100%;");
    menu_widget->setContentsMargins(0, 0, 0, 0);
    menu_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainMenu = new MainMenu(this, this);
    menu = new QVBoxLayout(this);
    user_layout = new QVBoxLayout(this) ;
    transaction_layout = new QVBoxLayout(this);
    card_layout = new QVBoxLayout(this);
    chat_layout = new QVBoxLayout(this);

    QIcon user_icon("icons\\menu\\avatar.png");
    user_btn = new QPushButton(this);
    user_btn->setIcon(user_icon);
    user_btn->setIconSize(QSize(35, 35));
    user_btn->setFlat(true);
    user_btn->setStyleSheet("background: 0;"
                            "border: 0;");
    user_btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    user_btn->setCursor(Qt::PointingHandCursor);

    user_layout->addWidget(user_btn, 0, Qt::AlignCenter);
    user_layout->addStretch();
    user_layout->setSpacing(2);

    menu->addLayout(user_layout);

    QIcon transaction_icon("icons\\menu\\transaction.png");
    transaction_btn = new QPushButton(this);
    transaction_btn->setIcon(transaction_icon);
    transaction_btn->setIconSize(QSize(35, 35));
    transaction_btn->setFlat(true);
    transaction_btn->setStyleSheet("background: 0;"
                               "border: 0;");
    transaction_btn->setCursor(Qt::PointingHandCursor);


    transaction_layout->addWidget(transaction_btn, 0, Qt::AlignCenter);
    transaction_layout->addStretch();
    transaction_layout->setSpacing(2);

    menu->addLayout(transaction_layout);

    QIcon card_icon("icons\\menu\\white_card.png");
    card_btn = new QPushButton(this);
    card_btn->setIcon(card_icon);
    card_btn->setIconSize(QSize(35, 35));
    card_btn->setFlat(true);
    card_btn->setStyleSheet("background: 0;"
                                "border: 0;");
    card_btn->setCursor(Qt::PointingHandCursor);

    card_layout->addWidget(card_btn, 0, Qt::AlignCenter);
    card_layout->addStretch();
    card_layout->setSpacing(2);

    menu->addLayout(card_layout);

    // QIcon chat_icon("icons\\menu\\chat.png");
    // chat_btn = new QPushButton(this);
    // chat_btn->setIcon(chat_icon);
    // chat_btn->setIconSize(QSize(35, 35));
    // chat_btn->setFlat(true);
    // chat_btn->setStyleSheet("background: 0;"
    //                         "border: 0;");
    // chat_btn->setCursor(Qt::PointingHandCursor);

    // chat_label = new QLabel(this);
    // chat_label->setText("Чат");
    // chat_label->setFont(font);

    // chat_layout->addWidget(chat_btn, 0, Qt::AlignCenter);
    // chat_layout->addWidget(chat_label, 0, Qt::AlignCenter);
    // chat_layout->addStretch();
    // chat_layout->setSpacing(2);

    // menu->addLayout(chat_layout);

    menu_widget->setLayout(menu);
    menu_widget->setFixedWidth(80);
    Layout2->addWidget(menu_widget);
    Layout2->addWidget(mainMenu, 1);
    Layout->addLayout(Layout2);
    Layout->setContentsMargins(0, 0, 0, 0);
    Layout->setSpacing(0);

    user_layout->setContentsMargins(0, 0, 0, 0);
    transaction_layout->setContentsMargins(0, 0, 0, 0);
    card_layout->setContentsMargins(0, 0, 0, 0);
    chat_layout->setContentsMargins(0, 0, 0, 0);
}
