#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <qchart.h>
#include <QtCharts/QLineSeries.h>
#include <QtCharts/qchartview.h>
#include <QtCharts/qdatetimeaxis.h>
#include <QtCharts/QValueAxis>
#include <QDateTime>

#include <vector>
#include <algorithm>

struct Transaction {
    std::string date;
    double price;
};

class Graphic
{
public:
    Graphic(std::vector<Transaction> cast, std::vector<Transaction> income);
    QChartView* get_chart();
private:
    QLineSeries* series1;
    QLineSeries* series2;

    QChart* chart;
    QChartView* chart_view;
    QValueAxis* axisY;
    QDateTimeAxis* axisX;

    QFont axis_font;
};

#endif // GRAPHIC_H
