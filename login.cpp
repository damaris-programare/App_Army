#include "login.h"
#include "mainwindow.h"
#include "ui_login.h"
#include "sqlstruct.h"
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QString>
#include "register.h"
#include "ui_user.h"


login::login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);

    log.connectToDb();

    u.resize(size_U1);
    u.reserve(size_U1);

    u[ID_id_user]="id_user";
    u[ID_userName]="userName";
    u[ID_password]="password";
    u[ID_role]="role";

}

login::~login()
{
    delete ui;

}

void login::on_pushButton_Login_clicked()
{
    QString UserName = ui->lineEdit_UserName->text();
    QString Password = ui->lineEdit_Password->text();

    if (UserName.isEmpty() || Password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both username and password.");
        return;
    }

    // Prepare the query to fetch password and role in one go
    QSqlQuery qry;
    qry.prepare("SELECT password, role FROM Users WHERE userName = :username");
    qry.bindValue(":username", UserName);

    if (!qry.exec()) {
        qDebug() << "Error reading Users" << qry.lastError();
        QMessageBox::critical(this, "Database Error", "Failed to query database: " + qry.lastError().text());
        return;
    }

    if (qry.next()) {
        QString storedPassword = qry.value(0).toString();
        int role = 0;
        role = qry.value(1).toInt();

        qDebug() << "Stored Password:" << storedPassword;
        qDebug() << "Entered Password:" << Password;
        qDebug() << "Role from DB:" << role;

        if (Password == storedPassword) {
            // Successful login
            if (role == 1) { // 1 = Admin
                QMessageBox::information(this, "Login", "Login Success for Admin.");
                this->hide();
                MainWindow *mainwindow = new MainWindow();
                mainwindow->show();
            } else if (role == 2) { // 2 = User
                QMessageBox::information(this, "Login", "Login Success for basic user.");
                this->hide();
                UI_user *ui_user = new UI_user();
                ui_user->show();
            } else {
                QMessageBox::warning(this, "Error", "Invalid role. Contact support.");
            }
        } else {
            QMessageBox::warning(this, "Error", "Invalid username or password.");
        }
    } else {
        QMessageBox::warning(this, "Error", "Invalid username or password.");
    }
}

void login::on_pushButton_Cancel_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Army", "Are you sure to close the aplication?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        QApplication::quit();
    }
}

void login::on_pushButton_clicked()
{
    Register *register_ = new Register();
    this->hide();
    register_->show();
}
