#include "graphic.h"
#include <iostream>

Graphic::Graphic(std::vector<Transaction> cast, std::vector<Transaction> income) {
    series1 = new QLineSeries();
    series2 = new QLineSeries();

    QPen pen = series2->pen();
    pen.setWidth(2);
    series1->setPen(pen);
    series2->setPen(pen);

    series1->setPointsVisible(true);
    series2->setPointsVisible(true);
    series1->setColor(QColor(21, 188, 230));
    series2->setColor(QColor(250, 7, 7));

    double max_cast = 0;
    double max_income = 0;

    for (Transaction t: cast) {
        series1->append(QDateTime::fromString(t.date.c_str(), "yyyy-MM-dd").toMSecsSinceEpoch(), static_cast<qreal>(t.price));
        if (t.price > max_cast) {
            max_cast = t.price;
        }
    }

    for (Transaction t: income) {
        series2->append(QDateTime::fromString(t.date.c_str(), "yyyy-MM-dd").toMSecsSinceEpoch(), static_cast<qreal>(t.price));
        if (t.price > max_cast) {
            max_income = t.price;
        }
    }

    int max_price = std::ceil(std::max(max_cast, max_income));
    std::cout << max_price << std::endl;

    chart = new QChart();
    chart->setBackgroundBrush(QBrush(QColor(16,16,28)));
    chart->setPlotAreaBackgroundBrush(QBrush(QColor(16,16,28)));
    chart->setPlotAreaBackgroundVisible(true);
    chart->addSeries(series1);
    chart->addSeries(series2);
    chart->createDefaultAxes();
    chart->axes(Qt::Vertical).first()->setRange(0, max_price);
    chart->axes(Qt::Horizontal).first()->setRange(0, 11);
    chart->setVisible(true);
    chart->legend()->hide();

    axisX = new QDateTimeAxis();
    QBrush axisBrush(Qt::white);
    axisX->setLabelsBrush(axisBrush);

    axisX->setTickCount(12);
    axisX->setFormat("MMM");

    QDateTime min_date = QDateTime::fromString("2024-01-01", "yyyy-MM-dd");
    QDateTime max_date = QDateTime::fromString("2024-12-31", "yyyy-MM-dd");
    axisX->setRange(min_date, max_date);

    axis_font.setPointSize(10);
    axisX->setLabelsFont(axis_font);
    axisX->setGridLineVisible(false);

    axisY = new QValueAxis();
    axisY->setLabelsFont(axis_font);
    axisY->setLabelsBrush(axisBrush);
    axisY->setTickCount(6);

    chart->setAxisX(axisX, series1);
    chart->setAxisY(axisY, series1);
    chart->setAxisX(axisX, series2);
    chart->setAxisY(axisY, series2);

    series1->attachAxis(axisX);
    series1->attachAxis(axisY);
    series2->attachAxis(axisX);
    series2->attachAxis(axisY);

    chart_view = new QChartView(chart);
    chart_view->setRenderHint(QPainter::Antialiasing);
    chart_view->setVisible(true);
    chart_view->setFixedHeight(500);
    chart_view->setAlignment(Qt::AlignTop);
}

QChartView* Graphic::get_chart() {
    return chart_view;
}
