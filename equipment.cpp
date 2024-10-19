#include "equipment.h"
#include "ui_equipment.h"

equipment::equipment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::equipment)
{
    ui->setupUi(this);
}

equipment::~equipment()
{
    delete ui;
}
