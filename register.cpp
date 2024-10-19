#include "register.h"
#include "login.h"
#include "ui_register.h"
#include "sqlstruct.h"
#include <QString>
#include <QSqlQuery>
#include <QDate>
#include <QDebug>
#include <QSqlError>
#include <QApplication>
#include <QFileInfo>
#include <QMessageBox>
#include <QUuid>

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);

    u.resize(size_U1);
    u.reserve(size_U1);

    u[ID_id_user]="id_user";
    u[ID_userName]="userName";
    u[ID_password]="password";
    u[ID_role]="role";

}

Register::~Register()
{
    delete ui;
}

void Register::on_pushButton_Back_clicked()
{
    login *login_ = new login();
    this->hide();
    login_->show();

}

void Register::on_pushButton_SignUp_clicked()
{
    QString UserName = ui->lineEdit_UserName_R->text();
    QString Password = ui->lineEdit_Password_R->text();
    QString ConfirmPassword = ui->lineEdit_Confirm_pass_R->text();

    if (UserName.isEmpty() || Password.isEmpty() || ConfirmPassword.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "All fields are required.");
        return;
    }

    if (Password != ConfirmPassword) {
        QMessageBox::warning(this, "Input Error", "Passwords do not match.");
        return;
    }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM users WHERE " + u[ID_userName] + " = :username");
    checkQuery.bindValue(":username", UserName);

    if (!checkQuery.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to query database: " + checkQuery.lastError().text());
        return;
    }

    checkQuery.next();
    if (checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Registration Error", "Username already exists. Please choose a different username.");
        return;
    }



    QSqlQuery maxIdQuery;
    maxIdQuery.prepare("SELECT IFNULL(MAX("+ u[ID_id_user] + "), 0) FROM users");

    if (!maxIdQuery.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to query database for max ID: " + maxIdQuery.lastError().text());
        return;
    }

    maxIdQuery.next();
    int newUserID = maxIdQuery.value(0).toInt() + 1;

    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO users ("+ u[ID_id_user] + ", " + u[ID_userName] + "," + u[ID_password] + "," +u[ID_role]+") VALUES (:id, :username, :password, :role)");
    insertQuery.bindValue(":id", newUserID);
    insertQuery.bindValue(":username", UserName);
    insertQuery.bindValue(":password", Password);
    insertQuery.bindValue(":role", 2);

    if (!insertQuery.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to register user: " + insertQuery.lastError().text());
    } else {
        QMessageBox::information(this, "Registration Successful", "User registered successfully.");
        login *login_ = new login();
        this->hide();
        login_->show();
    }
}
