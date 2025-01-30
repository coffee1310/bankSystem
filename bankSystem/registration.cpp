#include "registration.h"

Registration::Registration(QWidget *parent, MainWidget* mainWidget) : QWidget(parent) {
    QVBoxLayout* Layout = new QVBoxLayout;

    font.setPixelSize(25);
    input_font.setPixelSize(15);

    error = new QLabel;
    error->setStyleSheet("color: red");
    error->setAlignment(Qt::AlignCenter);

    title = new QLabel;
    title->setFont(font);
    title->setText("Регистрация");
    title->setAlignment(Qt::AlignLeft);
    title->setStyleSheet("color: white;");

    FIO = new QLineEdit;
    phone_number = new QLineEdit;
    email = new QLineEdit;
    password = new QLineEdit;
    submit_registration = new QPushButton;


    FIO->setFixedSize(330, 40);
    FIO->setStyleSheet("QLineEdit {"
                       "border: 2px solid #2C323A;"
                       "border-radius: 5px;"
                       "background: #1A1F25;"
                       "color: #707070"
                       "}"
                       "QLineEdit::focus {"
                       "color: white;"
                       "}");
    FIO->setPlaceholderText("ФИО");

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
    phone_number->move(0, 0);
    phone_number->setPlaceholderText("Номер телефона");

    email->setFixedSize(330, 40);
    email->setStyleSheet("QLineEdit {"
                         "border: 2px solid #2C323A;"
                         "border-radius: 5px;"
                         "background: #1A1F25;"
                         "color: #707070"
                         "}"
                         "QLineEdit::focus {"
                         "color: white;"
                         "}");
    email->move(0, 0);
    email->setPlaceholderText("Email");

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

    submit_registration->setStyleSheet("QPushButton {"
                                       "background: #455BCC;"
                                       "border: 0;"
                                       "border-radius: 5px;"
                                       "color: white;"
                                       "}"
                                       "QPushButton:hover {"
                                       ""
                                       "}");
    submit_registration->setFixedSize(360, 50);
    submit_registration->setText("Зарегистрироваться");
    submit_registration->setCursor(Qt::PointingHandCursor);
    connect(submit_registration, &QPushButton::clicked, this, user_creating);

    FIO->setFont(input_font);
    phone_number->setFont(input_font);
    email->setFont(input_font);
    password->setFont(input_font);
    submit_registration->setFont(input_font);

    QPixmap user_icon("icons\\forms\\user.png");
    QPixmap phone_icon("icons\\forms\\phone.png");
    QPixmap email_icon("icons\\forms\\email.png");
    QPixmap password_icon("icons\\forms\\password.png");

    auto new_user_icon = user_icon.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    auto new_phone_icon = phone_icon.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    auto new_email_icon = email_icon.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    auto new_password_icon = password_icon.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    user_label = new QLabel;
    phone_label = new QLabel;
    email_label = new QLabel;
    password_label = new QLabel;

    user_label->setAlignment(Qt::AlignRight);
    phone_label->setAlignment(Qt::AlignRight);
    email_label->setAlignment(Qt::AlignRight);
    password_label->setAlignment(Qt::AlignRight);

    user_label->setFixedSize(30, 30);
    phone_label->setFixedSize(30, 30);
    email_label->setFixedSize(30, 30);
    password_label->setFixedSize(30, 30);

    user_label->setPixmap(new_user_icon);
    phone_label->setPixmap(new_phone_icon);
    email_label->setPixmap(new_email_icon);
    password_label->setPixmap(new_password_icon);

    title_layout = new QHBoxLayout();
    user_layout = new QHBoxLayout();
    phone_layout = new QHBoxLayout();
    email_layout = new QHBoxLayout();
    password_layout = new QHBoxLayout();
    submint_btn_layout = new QHBoxLayout();

    title_layout->addWidget(title);
    user_layout->addWidget(user_label);
    user_layout->addWidget(FIO);
    phone_layout->addWidget(phone_label);
    phone_layout->addWidget(phone_number);
    email_layout->addWidget(email_label);
    email_layout->addWidget(email);
    password_layout->addWidget(password_label);
    password_layout->addWidget(password);
    submint_btn_layout->addWidget(submit_registration);

    title_layout->setAlignment(Qt::AlignCenter);
    user_layout->setAlignment(Qt::AlignCenter);
    phone_layout->setAlignment(Qt::AlignCenter);
    email_layout->setAlignment(Qt::AlignCenter);
    password_layout->setAlignment(Qt::AlignCenter);
    submint_btn_layout->setAlignment(Qt::AlignCenter);

    Layout->addLayout(title_layout);
    Layout->addLayout(user_layout);
    Layout->addLayout(phone_layout);
    Layout->addLayout(email_layout);
    Layout->addLayout(password_layout);
    Layout->addLayout(submint_btn_layout);
    Layout->addWidget(error);
    Layout->setAlignment(Qt::AlignCenter);
    setLayout(Layout);
}

Registration::~Registration()
{
    delete FIO;
    delete phone_number;
    delete password;
    delete submit_registration;
}

Registration::go_authorization()
{
    delete this;
    return 0;
}

Registration::user_creating() {
    CSimpleIniA ini;
    ini.SetUnicode();
    if (ini.LoadFile("user.ini") == SI_FILE) {
        std::cerr << "Ошибка при загрузке INI файла." << std::endl;
        return -1;
    }

    std::string FIO = this->FIO->text().toStdString();
    std::string password = this->password->text().toStdString();
    std::string phone_number = this->phone_number->text().toStdString();
    std::string email = this->email->text().toStdString();
    const char* section = "User";
    const char* key;

    key = "phone_number";
    ini.SetValue(section, key, phone_number.c_str());
    key = "FIO";
    ini.SetValue(section, key, FIO.c_str());
    ini.SaveFile("user.ini");

    connection::send_msg("create_user");
    connection::send_msg(FIO);
    connection::send_msg(phone_number);
    connection::send_msg(email);
    connection::send_msg(password);

    std::string response = connection::get_msg();

    if (strcmp(response.c_str(), "user created") == 0) {
        std::cout << "created" << std::endl;
        error->setText("");
    } else if (strcmp(response.c_str(), "error email") == 0) {
        std::cout << "Email" << std::endl;
        error->setText("Неправильно указан email");
    } else if (strcmp(response.c_str(), "error phone") == 0) {
        error->setText("Неправильно указан номер телефона");
    } else if (strcmp(response.c_str(), "error FIO") == 0) {
        error->setText("ФИО не должно содержать цифр");
    }

    return 0;
}
