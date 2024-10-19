#ifndef LOGIN_H
#define LOGIN_H

#include "mainwindow.h"
#include "sqlstruct.h"
#include <QMainWindow>
#include <QString>
#include <QStringList>

namespace Ui {
class login;
}

class login : public QMainWindow
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();

    enum U1 {
        ID_id_user=0,
        ID_userName,
        ID_password,
        ID_role,
        size_U1
            };
    QVector <QString> u;
private slots:
    void on_pushButton_Login_clicked();

    void on_pushButton_Cancel_clicked();

    void on_pushButton_clicked();

private:
    Ui::login *ui;
    SqlStruct log;
};

#endif // LOGIN_H
