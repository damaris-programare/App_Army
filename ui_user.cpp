#include "ui_user.h"
#include "ui_ui_user.h"
#include "login.h"
#include "sqlstruct.h"
#include "mainwindow.h"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QDialog>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QDate>
#include <QMap>

using namespace QtCharts;

UI_user::UI_user(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UI_user),
    currentChartView(nullptr),
    previousChartView(nullptr)
{
    ui->setupUi(this);
    o2.getPaymentContent(ui->tableViewPayment);
    o2.getEquipmentContent(ui->tableViewEquipment);
    setupUI();
    hideButtons(); // Hide buttons initially
}

UI_user::~UI_user() {
    delete ui;
    if (currentChartView) {
        delete currentChartView;
        currentChartView = nullptr;
    }
    if (previousChartView) {
        delete previousChartView;
        previousChartView = nullptr;
    }
}

void UI_user::setupUI() {
    mainLayout = new QVBoxLayout(this);

    showCountryExpensesButton = new QPushButton("Show Country Expenses", this);
    mainLayout->addWidget(showCountryExpensesButton);
    connect(showCountryExpensesButton, &QPushButton::clicked, this, &UI_user::on_pushButton_tari_clicked);

    showTotalExpensesButton = new QPushButton("Show total Expenses by year", this);
    mainLayout->addWidget(showTotalExpensesButton);
    connect(showTotalExpensesButton, &QPushButton::clicked, this, &UI_user::on_pushButton_clicked);

    backButton = new QPushButton("Back", this);
    backButton->setEnabled(false);
    mainLayout->addWidget(backButton);

    connect(backButton, &QPushButton::clicked, this, &UI_user::onBackButtonClicked);
}

void UI_user::hideButtons() {
    showCountryExpensesButton->hide();
    showTotalExpensesButton->hide();
    backButton->hide();
}

void UI_user::showButtons() {
    showCountryExpensesButton->show();
    showTotalExpensesButton->show();
    backButton->show();
}

void UI_user::onBackButtonClicked() {
    previousChartView = currentChartView;
    if (previousChartView) {
        if (currentChartView) {
            mainLayout->removeWidget(currentChartView);
            delete currentChartView;
            currentChartView = nullptr;
        }
        //currentChartView = previousChartView;
        previousChartView = nullptr;
        backButton->setEnabled(false);
    }

    // Hide buttons when back button is clicked
    hideButtons();
}

void UI_user::on_pushButton_clicked() {
    previousChartView = currentChartView;
    auto result = getBarSeriesFromDatabase();
    QBarSeries *series = result.first;
    qreal maxValue = result.second;
    displayBarChart(series, maxValue);

    backButton->setEnabled(true);
    showButtons(); // Show buttons when displaying the chart

    qDebug() << "Barchart page shown 1";
}

void UI_user::on_pushButton_Back_clicked() {
    login *login_ = new login();
    this->hide();
    login_->show();
}

QPair<QBarSeries*, qreal> UI_user::getBarSeriesFromDatabase() {
    QBarSeries *series = new QBarSeries();
    int currentYear = QDate::currentDate().year();
    int startYear = currentYear - 14; // 15 years ago

    QMap<int, qreal> yearlySums;
    for (int year = startYear; year <= currentYear; ++year) {
        yearlySums[year] = 0;
    }

    QSqlQuery equipmentQuery;
    QString queryStr = QString(
        "SELECT strftime('%Y', substr(data, 7, 4) || '-' || substr(data, 4, 2) || '-' || substr(data, 1, 2)) AS year, SUM(CAST(pret AS REAL) * CAST(numarBucati AS REAL)) "
        "FROM Equipment "
        "WHERE strftime('%Y', substr(data, 7, 4) || '-' || substr(data, 4, 2) || '-' || substr(data, 1, 2)) BETWEEN '%1' AND '%2' "
        "GROUP BY year"
    ).arg(startYear).arg(currentYear);

    if (!equipmentQuery.exec(queryStr)) {
        qDebug() << "Failed to execute query:" << equipmentQuery.lastError().text();
        return qMakePair(series, 0.0);
    }

    while (equipmentQuery.next()) {
        int year = equipmentQuery.value(0).toInt();
        qreal sum = equipmentQuery.value(1).toDouble() / 1000000.0; // Convert to millions
        yearlySums[year] = sum;
    }

    QBarSet *yearlySet = new QBarSet("Yearly Equipment Sales");
    qreal maxValue = 0;
    for (int year = startYear; year <= currentYear; ++year) {
        qreal value = yearlySums[year];
        *yearlySet << value;
        if (value > maxValue) {
            maxValue = value;
        }
    }
    series->append(yearlySet);
    return qMakePair(series, maxValue);
}

void UI_user::displayBarChart(QBarSeries* series, qreal maxValue) {
    if (currentChartView != nullptr && mainLayout->indexOf(currentChartView) != -1) {
        mainLayout->removeWidget(currentChartView);
        delete currentChartView;
        currentChartView = nullptr;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Yearly Equipment Sales");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    int currentYear = QDate::currentDate().year();
    for (int year = currentYear - 14; year <= currentYear; ++year) {
        categories << QString::number(year);
    }

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, maxValue);
    axisY->setLabelFormat("%.0f M"); // Set label format to display values in millions
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    currentChartView = new QChartView(chart);
    currentChartView->setRenderHint(QPainter::Antialiasing);

    mainLayout->addWidget(currentChartView);

    qDebug() << "Barchart page shown 2";
}

void UI_user::on_pushButton_tari_clicked() {
    previousChartView = currentChartView;
    auto result = getCountryExpensesFromDatabase();
    displayCountryExpensesBarChart(result.series, result.maxValue, result.categoryNames);
    backButton->setEnabled(true);
    showButtons(); // Show buttons when displaying the chart

    qDebug() << "Expanses page shown 1";
}

UI_user::CountryExpensesData UI_user::getCountryExpensesFromDatabase() {
    QBarSeries *series = new QBarSeries();
    int currentYear = QDate::currentDate().year();
    int startYear = currentYear - 10; // Last 10 years

    QMap<QString, qreal> countryExpenses;

    QSqlQuery paymentQuery;
    QString queryStr = QString(
        "SELECT vanzatorName, SUM(CAST(pret AS REAL)) "
        "FROM Payment "
        "WHERE strftime('%Y', substr(data, 7, 4) || '-' || substr(data, 4, 2) || '-' || substr(data, 1, 2)) BETWEEN '%1' AND '%2' "
        "GROUP BY vanzatorName"
    ).arg(startYear).arg(currentYear);

    if (!paymentQuery.exec(queryStr)) {
        qDebug() << "Failed to execute query:" << paymentQuery.lastError().text();
        return { series, 0.0, QStringList() };
    }

    while (paymentQuery.next()) {
        QString country = paymentQuery.value(0).toString();
        qreal sum = paymentQuery.value(1).toDouble();
        countryExpenses[country] = sum;
    }

    QBarSet *countrySet = new QBarSet("Country Expenses");
    QStringList categoryNames;
    qreal maxValue = 0;
    for (auto it = countryExpenses.begin(); it != countryExpenses.end(); ++it) {
        qreal value = it.value();
        *countrySet << value;
        categoryNames << it.key();
        if (value > maxValue) {
            maxValue = value;
        }
    }
    series->append(countrySet);
    return { series, maxValue, categoryNames };
}

void UI_user::displayCountryExpensesBarChart(QBarSeries* series, qreal maxValue, const QStringList &categoryNames) {
    if (currentChartView != nullptr && mainLayout->indexOf(currentChartView) != -1) {
        mainLayout->removeWidget(currentChartView);
        delete currentChartView;
        currentChartView = nullptr;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Country Expenses for the Last 10 Years");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categoryNames);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, maxValue);
    axisY->setLabelFormat("%.0f M"); // Set label format to display values in millions
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    currentChartView = new QChartView(chart);
    currentChartView->setRenderHint(QPainter::Antialiasing);

    mainLayout->addWidget(currentChartView);

    qDebug() << "Expanses page shown 3";
}
