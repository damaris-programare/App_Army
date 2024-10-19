#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <QDialog>

namespace Ui {
class equipment;
}

class equipment : public QDialog
{
    Q_OBJECT

public:
    explicit equipment(QWidget *parent = 0);
    ~equipment();

private:
    Ui::equipment *ui;
};

#endif // EQUIPMENT_H
