#include "graphics.h"
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>

Graphics::Graphics(QWidget *parent) :
    QDialog(parent),
    //ui(new Ui::Graphics),
    chartView(new QChartView(this))
{
    //ui->setupUi(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
    setLayout(layout);
}

Graphics::~Graphics()
{
    delete ui;
}

void Graphics::displayBarChart(QBarSeries *series, qreal maxValue)
{
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistical Chart Example");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Crearea axelor
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append("Equipment");

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, maxValue); // Setează intervalul axei Y

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
}
