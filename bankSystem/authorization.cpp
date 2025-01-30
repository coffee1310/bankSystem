#include "authorization.h"

Authorization::Authorization(QWidget *parent, MainWidget* mainwidget) {
    this->mainWidget = mainwidget;

    font.setPixelSize(25);
    input_font.setPixelSize(15);

    title = new QLabel;
    title->setFont(font);

    title->setText("Авторизация");
    title->setFont(font);
    title->setStyleSheet("color: white");
    title->setAlignment(Qt::AlignCenter);

    Layout = new QVBoxLayout;
    phone_number = new QLineEdit;
    password = new QLineEdit;
    submit_authorization = new QPushButton;

    phone_number->setFixedSize(330, 40);
    phone_number->setStyleSheet("QLineEdit {"
                               "border: 2px solid #2C323A;"
                               "border-radius: 5px;"
                               "background: #1A1F25;"
                               "color: #707070"
                               "}"
                               "QLineEdit::focus {"
                               "color: white;"
                               "}");

    phone_number->setPlaceholderText("Номер телефона");

    std::string loaded_phone_number = read_phone_number();
    phone_number->setText(QString::fromStdString(loaded_phone_number));

    password->setFixedSize(330, 40);
    password->setStyleSheet("QLineEdit {"
                            "border: 2px solid #2C323A;"
                            "border-radius: 5px;"
                            "background: #1A1F25;"
                            "color: #707070"
                            "}"
                            "QLineEdit::focus {"
                            "color: white;"
                            "}");
    password->setPlaceholderText("Пароль");

    submit_authorization->setStyleSheet("QPushButton {"
                                        "background: #455BCC;"
                                        "border: 0;"
                                        "border-radius: 5px;"
                                        "color: white;"
                                        "}"
                                        "QPushButton:hover {"
                                        ""
                                        "}");
    submit_authorization->setFixedSize(360, 50);
    submit_authorization->setText("Авторизоваться");
    submit_authorization->setCursor(Qt::PointingHandCursor);

    QPixmap phone_icon("icons\\forms\\phone.png");
    QPixmap password_icon("icons\\forms\\password.png");

    auto new_phone_icon = phone_icon.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    auto new_password_icon = password_icon.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    phone_label = new QLabel;
    password_label = new QLabel;

    phone_number->setFont(input_font);
    password->setFont(input_font);
    submit_authorization->setFont(input_font);

    phone_label->setAlignment(Qt::AlignRight);
    password_label->setAlignment(Qt::AlignRight);

    phone_label->setFixedSize(30, 30);
    password_label->setFixedSize(30, 30);

    phone_label->setPixmap(new_phone_icon);
    password_label->setPixmap(new_password_icon);

    title_layout = new QHBoxLayout();
    phone_layout = new QHBoxLayout();
    password_layout = new QHBoxLayout();
    submint_btn_layout = new QHBoxLayout();

    title_layout->addWidget(title);
    phone_layout->addWidget(phone_label);
    phone_layout->addWidget(phone_number);
    password_layout->addWidget(password_label);
    password_layout->addWidget(password);
    submint_btn_layout->addWidget(submit_authorization);

    Layout->addLayout(title_layout);
    Layout->addLayout(phone_layout);
    Layout->addLayout(password_layout);
    Layout->addLayout(submint_btn_layout);
    Layout->setAlignment(Qt::AlignCenter);
    setLayout(Layout);
    connect(submit_authorization, &QPushButton::clicked, this, user_authorization);
}


const char* Authorization::read_phone_number() {
    CSimpleIniA ini;
    ini.SetUnicode();
    if (ini.LoadFile("user.ini") == SI_FILE) {
        std::cerr << "Ошибка при загрузке INI файла." << std::endl;
        return "";
    }

    const char* section = "user";
    const char* key;
    const char* value;
    key = "phone_number";
    value = ini.GetValue(section, key, "");
    return value;
}

Authorization::~Authorization()
{
    delete phone_number;
    delete password;
    delete submit_authorization;
}

Authorization::user_authorization() {
    connection::send_msg("login");

    std::string phone_number_str = this->phone_number->text().toStdString();
    std::string password_str = this->password->text().toStdString();

    connection::send_msg(phone_number_str);
    connection::send_msg(password_str);

    std::string response = connection::get_msg();
    if (response == "you are logged in") {
        this->phone_number->setText("111111");
        mainWidget->switch_to_main_menu();
        return 0;
    }

    return 1;
}
