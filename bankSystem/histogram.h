#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QChart>
#include <QDateTime>
#include <QStringList>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>

class Histogram
{
public:
    Histogram(std::map<std::string, double> transactions_types);
    QChartView* get_chart();

private:
    QBarSet* set0;
    QBarSeries* series;
    QChart* chart;
    QStringList categories;
    QBarCategoryAxis *axisX;
    QValueAxis *axisY;

    QChartView* chartView;
};

#endif // HISTOGRAM_H
