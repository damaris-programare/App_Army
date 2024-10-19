#ifndef UI_USER_H
#define UI_USER_H

#include <QDialog>
#include "sqlstruct.h"
#include "graphics.h"

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QDate>
#include <QMap>
#include <algorithm>
#include <QPushButton>
#include <QPair>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class UI_user;
}

class UI_user : public QDialog
{
    Q_OBJECT

public:
    explicit UI_user(QWidget *parent = 0);
    ~UI_user();

private slots:
    void on_pushButton_clicked();
    void onBackButtonClicked();
    //void onShowCountryExpensesClicked();
    void on_pushButton_tari_clicked();
    void on_pushButton_Back_clicked();

private:
    Ui::UI_user *ui;
    SqlStruct o2;
    QVBoxLayout *mainLayout;
    Graphics *graphicsWindow;
    QPushButton *backButton; // Pointer to the Back button
    QPushButton *showCountryExpensesButton;
    QPushButton *showTotalExpensesButton;
    QChartView *currentChartView = nullptr; // Pointer to the current chart view
    QChartView *previousChartView = nullptr; // Pointer to the previous chart view

    void setupUI();
    QPair<QBarSeries*, qreal> getBarSeriesFromDatabase();
    void displayBarChart(QBarSeries* series, qreal maxValue);
    void displayCountryExpensesBarChart(QBarSeries* series, qreal maxValue, const QStringList &categoryNames);
    void hideButtons();
    void showButtons();
    struct CountryExpensesData {
        QBarSeries *series;
        qreal maxValue;
        QStringList categoryNames;
    };
    CountryExpensesData getCountryExpensesFromDatabase();
};

#endif // UI_USER_H
