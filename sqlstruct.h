#ifndef SQLSTRUCT_H
#define SQLSTRUCT_H
#include <QSqlDatabase>
#include <QString>
#include <QStringList>
#include <QDate>
#include <QTableWidget>
#include <QGraphicsView>



class SqlStruct:public QObject
{
    //Q_OBJECT
public:
    SqlStruct();
    virtual ~SqlStruct();
    bool connectToDb();
    void getPaymentContent(QTableView *tableView);
    void insertTablePayment(QLineEdit *factura, QLineEdit *vanzator, QLineEdit *cumparator, QLineEdit *pret, QLineEdit *data);
    void updateTablePayment(QLineEdit *factura , QLineEdit *vanzator, QLineEdit *cumparator, QLineEdit *pret, QLineEdit *data );
    void deleteTablePayment(QLineEdit *factura, QLineEdit *vanzator, QLineEdit *cumparator, QLineEdit *pret, QLineEdit *data);

    void getEquipmentContent(QTableView *tableView);
    void insertTableEquipment(QLineEdit *id_echipament, QLineEdit *denumire, QLineEdit *nr_bucati, QLineEdit *pret_echipament, QLineEdit *data_equipment);
    void updateTableEquipment(QLineEdit *id_echipament, QLineEdit *denumire, QLineEdit *nr_bucati, QLineEdit *pret_echipament, QLineEdit *data_equipment );
    void deleteTableEquipment(QLineEdit *id_echipament, QLineEdit *denumire, QLineEdit *nr_bucati, QLineEdit *pret_echipament, QLineEdit *data_equipment );

    void getUsersContent(QTableView *tableView);
    void insertTableUsers(QLineEdit *id_user, QLineEdit *userName, QLineEdit *password, QLineEdit *role );
    void updateTableUsers(QLineEdit *id_user, QLineEdit *userName, QLineEdit *password, QLineEdit *role );
    void deleteTableUsers(QLineEdit *id_user, QLineEdit *userName, QLineEdit *password, QLineEdit *role );


    enum A1 {
        ID_F_INDEX=0,
        ID_C_INDEX,
        ID_V_INDEX,
        ID_P_INDEX,
        ID_D_INDEX,
        size_A1
            };

    QVector <QString> v;

    enum B1 {
        ID_Echipament=0,
        ID_Denumire,
        ID_Nr_bucati,
        ID_Pret_echipament,
        ID_Data,
        size_B1
            };

    QVector <QString> e;

    enum C1 {
        ID_User=0,
        ID_UserName,
        ID_Password,
        ID_Role,
        size_C1
            };

    QVector <QString> u;





private:
    QSqlDatabase db_con;


};

#endif // SQLSTRUCT_H
