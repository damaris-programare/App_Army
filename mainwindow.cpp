#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sqlstruct.h"
#include "login.h"
#include <QString>
#include <QSqlQuery>
#include <QDate>
#include <QDebug>
#include <QSqlError>
#include <QApplication>
#include <QFileInfo>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonRead_P_clicked()
{

    o1.getPaymentContent(ui->tableView);

}

void MainWindow::on_pushButtonInsert_P_clicked()
{
    o1.insertTablePayment(ui->factura,
                   ui->vanzator,
                   ui->cumparator,
                   ui->pret,
                   ui->data);
    o1.getPaymentContent(ui->tableView);

}

void MainWindow::on_pushButtonDelete_P_clicked()
{
    o1.deleteTablePayment(ui->factura,
                   ui->vanzator,
                   ui->cumparator,
                   ui->pret,
                   ui->data);
    o1.getPaymentContent(ui->tableView);
}

void MainWindow::on_pushButtonUpdate_P_clicked()
{
    o1.updateTablePayment(ui->factura,
                   ui->vanzator,
                   ui->cumparator,
                   ui->pret,
                   ui->data);
    o1.getPaymentContent(ui->tableView);
}

void MainWindow::on_pushButtonRead_E_clicked()
{
    o1.getEquipmentContent(ui->tableView);
}

void MainWindow::on_pushButtonInsert_E_clicked()
{
    o1.insertTableEquipment(ui->id_echipament,
                            ui->denumire,
                            ui->nr_bucati,
                            ui->pret_echipament,
                            ui->data_equipment);
    o1.getEquipmentContent(ui->tableView);
}

void MainWindow::on_pushButtonUpdate_E_clicked()
{
    o1.updateTableEquipment(ui->id_echipament,
                            ui->denumire,
                            ui->nr_bucati,
                            ui->pret_echipament,
                            ui->data_equipment);
    o1.getEquipmentContent(ui->tableView);
}

void MainWindow::on_pushButtonDelete_E_clicked()
{
    o1.deleteTableEquipment(ui->id_echipament,
                            ui->denumire,
                            ui->nr_bucati,
                            ui->pret_echipament,
                            ui->data_equipment);
    o1.getEquipmentContent(ui->tableView);
}

void MainWindow::on_pushButtonRead_U_clicked()
{
    o1.getUsersContent(ui->tableView);
}

void MainWindow::on_pushButtonInsert_U_clicked()
{
    o1.insertTableUsers(ui->id_user,
                        ui->userName,
                        ui->password,
                        ui->role);
    o1.getUsersContent(ui->tableView);
}

void MainWindow::on_pushButtonUpdate_U_clicked()
{
    o1.updateTableUsers(ui->id_user,
                        ui->userName,
                        ui->password,
                        ui->role);
    o1.getUsersContent(ui->tableView);
}

void MainWindow::on_pushButtonDelete_U_clicked()
{
    o1.deleteTableUsers(ui->id_user,
                        ui->userName,
                        ui->password,
                        ui->role);
    o1.getUsersContent(ui->tableView);
}

void MainWindow::on_pushButton_clicked()
{
    login *login_ = new login();
    this->hide();
    login_->show();
}
