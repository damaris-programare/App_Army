#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <QDialog>

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class Graphics;
}

class Graphics : public QDialog
{
    Q_OBJECT

public:
    explicit Graphics(QWidget *parent = 0);
    void displayBarChart(QBarSeries *series, qreal maxValue);
    ~Graphics();

private:
    Ui::Graphics *ui;
    QChartView *chartView;
};

#endif // GRAPHICS_H
