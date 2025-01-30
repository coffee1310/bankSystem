#include "histogram.h"

Histogram::Histogram(std::map<std::string, double> transactions_types){
    set0 = new QBarSet("Low");
    set0->setBorderColor(Qt::transparent);
    *set0 << 1 << 2 << 3 << 4 << 5 << 6;

    series = new QBarSeries();
    series->append(set0);

    chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setBackgroundBrush(QBrush(QColor(16,16,28)));
    chart->setPlotAreaBackgroundBrush(QBrush(QColor(16,16,28)));
    chart->setPlotAreaBackgroundVisible(true);

    for (const auto& t: transactions_types) {
        this->categories << QString::fromStdString(t.first);
    }

    axisX = new QBarCategoryAxis();
    axisX->append(this->categories);
    axisX->setGridLineVisible(false);

    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    axisY = new QValueAxis();
    axisY->setRange(0,15);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(false);
    chart->legend()->setAlignment(Qt::AlignBottom);

    chartView = new QChartView(chart);
    chartView->setFixedHeight(500);
    chartView->setRenderHint(QPainter::Antialiasing);
}

QChartView* Histogram::get_chart() {
    return chartView;
}
