#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sqlstruct.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_pushButtonRead_P_clicked();

    void on_pushButtonInsert_P_clicked();

    void on_pushButtonDelete_P_clicked();

    void on_pushButtonUpdate_P_clicked();

    void on_pushButtonRead_E_clicked();

    void on_pushButtonInsert_E_clicked();

    void on_pushButtonUpdate_E_clicked();

    void on_pushButtonDelete_E_clicked();

    void on_pushButtonRead_U_clicked();

    void on_pushButtonInsert_U_clicked();

    void on_pushButtonUpdate_U_clicked();

    void on_pushButtonDelete_U_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    SqlStruct o1;

};

#endif // MAINWINDOW_H
