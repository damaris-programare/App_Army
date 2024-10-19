#include "mainwindow.h"
#include "register.h"
#include <QApplication>
#include "login.h"
#include "sqlstruct.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    login log;
    log.show();

    return a.exec();
}
