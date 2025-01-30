#include "main_menu.h"


MainMenu::MainMenu(QWidget *parent, MainWidget* mainWidget)
{
    this->mainWidget = mainWidget;
    Layout = new QVBoxLayout();
    get_transactions("year");

    graphics = new QHBoxLayout();

    Graphic graphic(this->cost, this->income);
    QChartView* chart_view = graphic.get_chart();
    chart_view->setAlignment(Qt::AlignTop);

    get_transaction_types("year");

    Histogram histogram(transaction_types);

    QChartView* chart_view2 = histogram.get_chart();
    graphics->addWidget(chart_view);
    graphics->addWidget(chart_view2);

    Layout->addLayout(graphics);
    setLayout(Layout);
}

MainMenu::~MainMenu()
{

}

int MainMenu::get_transactions(std::string period) {
    connection::send_msg("send_transactions");
    connection::send_msg(period);

    std::string type;
    type = connection::get_msg();

    if (type == "Cost") {
        int count = std::stoi(connection::get_msg());
        for (int i = 0; i < count; i++) {
            Transaction t;
            t.date = connection::get_msg();
            t.price = std::stoi(connection::get_msg());
            this->cost.push_back(t);
        }
    }

    type = connection::get_msg();
    if (type == "Income") {
        int count = std::stoi(connection::get_msg());
        for (int i = 0; i < count; i++) {
            Transaction t;
            t.date = connection::get_msg();
            t.price = std::stoi(connection::get_msg());
            this->income.push_back(t);
        }
    }

    return 0;
}

int MainMenu::get_transaction_types(std::string period) {
    connection::send_msg("send_transaction_types");
    connection::send_msg("year");
    int types_count = std::stoi(connection::get_msg());

    for (int i = 0; i < types_count; i++) {
        std::string type = connection::get_msg();
        transaction_types[type] = std::stod(connection::get_msg());
    }

    return 0;
}









