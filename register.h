#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = 0);
    ~Register();

    enum U1 {
        ID_id_user=0,
        ID_userName,
        ID_password,
        ID_role,
        size_U1
            };

    QVector <QString> u;

private slots:
    void on_pushButton_Back_clicked();

    void on_pushButton_SignUp_clicked();

private:
    Ui::Register *ui;
};

#endif // REGISTER_H
