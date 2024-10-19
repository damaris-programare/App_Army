#include "sqlstruct.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QDate>
#include <QString>
#include <QApplication>
#include <QFileInfo>
#include <QMessageBox>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QSqlQueryModel>
#include <QLineEdit>


SqlStruct::SqlStruct()
{
    v.resize(size_A1);
    v.reserve(size_A1);

    v[ID_F_INDEX]="id_factura";
    v[ID_V_INDEX]="vanzatorName";
    v[ID_C_INDEX]="cumparatorName";
    v[ID_P_INDEX]="pret";
    v[ID_D_INDEX]="data";

    e.resize(size_B1);
    e.reserve(size_B1);

    e[ID_Echipament]="idEchipament";
    e[ID_Denumire]="denumireEchipament";
    e[ID_Nr_bucati]="numarBucati";
    e[ID_Pret_echipament]="pret";
    e[ID_Data]="data";

    u.resize(size_C1);
    u.reserve(size_C1);

    u[ID_User]="id_user";
    u[ID_UserName]="userName";
    u[ID_Password]="password";
    u[ID_Role]="role";


}

SqlStruct::~SqlStruct()
{
    if (db_con.isOpen())
    {
        db_con.close();
    }
}

bool SqlStruct::connectToDb()
{
    bool result = false;
    qDebug() << QSqlDatabase::drivers();

    db_con = QSqlDatabase::addDatabase( "QSQLITE" );
    db_con.setHostName("127.0.0.1");

    QString dbFilePath = QApplication::applicationDirPath()+ "/BDforArmy.sqlite";
    qDebug()<<dbFilePath;
    QFileInfo ff(dbFilePath );
    if(ff.exists()) {
        /// Baza de date există -> continuă cu conectarea
        db_con.open();

    } else {
        QMessageBox::critical(nullptr, "Eroare", "Baza de date lipsește!");
    }

    db_con.setDatabaseName(dbFilePath );

    if( !db_con.open() )
    {
        qDebug() << "Eroare la deschiderea bazei de date:" << db_con.lastError().text();
        QMessageBox::warning(nullptr,"Eroare","Baza de date nu se deschide!");
    }else{
        qDebug() << "Database Connected";
       // QMessageBox::information(nullptr,"Message","Database Connected!");
        result = true;
    }
    return result;
}



void SqlStruct::getPaymentContent(QTableView *tableView)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery *qry = new QSqlQuery(db_con);
    qry->prepare("SELECT * from Payment");
    qry->exec();
    model->setQuery(*qry);
    tableView->setModel(model);
    qDebug() << (model->rowCount());

    if(!qry->exec())
    {
        qDebug() << "eroare afisare Payment" << qry->lastError();
        QMessageBox::information(nullptr,"Army", "Display error!");
    }else
    {
        qDebug() << "Afisare cu succes!";
        //QMessageBox::information(nullptr,"Army", "Display successfully!");
    }
}

void SqlStruct::insertTablePayment(QLineEdit *factura, QLineEdit *vanzator, QLineEdit *cumparator, QLineEdit *pret, QLineEdit *data)
{
    QSqlQuery qry_insert(db_con);
    qry_insert.prepare("INSERT INTO Payment("+v[ID_F_INDEX]+", "+v[ID_V_INDEX]+","+ v[ID_C_INDEX]+", "+v[ID_P_INDEX]+", "+v[ID_D_INDEX]+") "
                                                                                                                   "VALUES (:f, :v, :c, :p, :d);");
    qry_insert.bindValue(":f", factura->text());
    qry_insert.bindValue(":v", vanzator->text());
    qry_insert.bindValue(":c", cumparator->text());
    qry_insert.bindValue(":p", pret->text());
    qry_insert.bindValue(":d", data->text());


    if(!qry_insert.exec())
    {
        if(qry_insert.lastError().text() == "UNIQUE constraint failed: Payment.id_factura Unable to fetch row") {
            QMessageBox::information(nullptr, "Army", "There is already an invoice with this ID!");
        } else if(factura->text().isEmpty() || vanzator->text().isEmpty() || cumparator->text().isEmpty() || pret->text().isEmpty() || data->text().isEmpty()) {
            QMessageBox::information(nullptr, "Army", "Fill in all the fields!");
        }
    }else{
        qDebug() << qry_insert.lastError();
        ///qDebug() << "inserted values.";
        QMessageBox::information(nullptr,"Army", "Inserted values!");
    }
}


void SqlStruct::updateTablePayment(QLineEdit *factura, QLineEdit *vanzator, QLineEdit *cumparator, QLineEdit *pret, QLineEdit *data)
{
    QSqlQuery qry_update;

    if (!vanzator->text().isEmpty()) {
        QString updateQuery = "UPDATE Payment SET " + v[ID_V_INDEX] + " = :noul_vanzator WHERE " + v[ID_F_INDEX] + " = :id_factura_de_actualizat";
        qry_update.prepare(updateQuery);
        qry_update.bindValue(":noul_vanzator", vanzator->text());
        qry_update.bindValue(":id_factura_de_actualizat", factura->text());

        if (!qry_update.exec()) {
            qDebug() << "Eroare la actualizare vanzator:" << qry_update.lastError().text();
            return;
        }
    }

    if (!cumparator->text().isEmpty()) {
        QString updateQuery = "UPDATE Payment SET " + v[ID_C_INDEX] + " = :noul_cumparator WHERE " + v[ID_F_INDEX] + " = :id_factura_de_actualizat";
        qry_update.prepare(updateQuery);
        qry_update.bindValue(":noul_cumparator", cumparator->text());
        qry_update.bindValue(":id_factura_de_actualizat", factura->text());

        if (!qry_update.exec()) {
            qDebug() << "Eroare la actualizare cumparator:" << qry_update.lastError().text();
            return;
        }
    }

    if (!pret->text().isEmpty()) {
        QString updateQuery = "UPDATE Payment SET " + v[ID_P_INDEX] + " = :nou_pret WHERE " + v[ID_F_INDEX] + " = :id_factura_de_actualizat";
        qry_update.prepare(updateQuery);
        qry_update.bindValue(":nou_pret", pret->text());
        qry_update.bindValue(":id_factura_de_actualizat", factura->text());

        if (!qry_update.exec()) {
            qDebug() << "Eroare la actualizare pret:" << qry_update.lastError().text();
            return;
        }
    }

    if (!data->text().isEmpty()) {
        QString updateQuery = "UPDATE Payment SET " + v[ID_D_INDEX] + " = :noua_data WHERE " + v[ID_F_INDEX] + " = :id_factura_de_actualizat";
        qry_update.prepare(updateQuery);
        qry_update.bindValue(":noua_data", data->text());
        qry_update.bindValue(":id_factura_de_actualizat", factura->text());

        if (!qry_update.exec()) {
            qDebug() << "Eroare la actualizare data:" << qry_update.lastError().text();
            return;
        }
    }


    if (!qry_update.exec()) {
        qDebug() << "Eroare la actualizare:" << qry_update.lastError().text();
        QMessageBox::information(nullptr, "Army", "Error updating!");
    } else {
        qDebug() << "Actualizare realizată cu succes!";
        QMessageBox::information(nullptr, "Army", "Update done successfully!");
    }
}



void SqlStruct::deleteTablePayment(QLineEdit *factura, QLineEdit *vanzator, QLineEdit *cumparator, QLineEdit *pret, QLineEdit *data )
{
    QSqlQuery qry_delete(db_con);

    ///delete id
    if(!factura->text().isEmpty()){
        qry_delete.prepare("DELETE FROM Payment WHERE "+v[ID_F_INDEX]+" = :ID;");
        qry_delete.bindValue(":ID", factura->text());
    }
    ///delete data
    if(!data->text().isEmpty()){
        qry_delete.prepare("DELETE FROM Payment WHERE "+v[ID_D_INDEX]+" = :d;");
        qry_delete.bindValue(":d", data->text());
    }
    ///delete vanzator && cumparator && pret
    if(!vanzator->text().isEmpty() && !cumparator->text().isEmpty() && !pret->text().isEmpty()){
        qry_delete.prepare("DELETE FROM Payment WHERE " + v[ID_V_INDEX] + " = :v AND " + v[ID_C_INDEX] + " = :c AND " + v[ID_P_INDEX] + " = :p");
        qry_delete.bindValue(":v", vanzator->text());
        qry_delete.bindValue(":c", cumparator->text());
        qry_delete.bindValue(":p", pret->text());
    }
    ///qry_delete.exec();

    if (!qry_delete.exec()) {
        QMessageBox::information(nullptr, "Army", "Error deleting record!");
        qDebug() <<  "Eroare la ștergerea înregistrării: " << qry_delete.lastError().text();
    } else {
        QMessageBox::information(nullptr, "Army", "Delete registration successfully completed!");
        qDebug() << "Stergere inregistrare realizată cu succes!";
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SqlStruct::getEquipmentContent(QTableView *tableView)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery *qry = new QSqlQuery(db_con);
    qry->prepare("SELECT * from Equipment");
    qry->exec();
    model->setQuery(*qry);
    tableView->setModel(model);
    qDebug() << (model->rowCount());

    if(!qry->exec())
    {
        qDebug() << "eroare afisare  table Equipment" << qry->lastError();
        QMessageBox::information(nullptr,"Army", "Display error!");
    }else
    {
        qDebug() << "Afisare cu succes table Equipment!";
        //QMessageBox::information(nullptr,"Army", "Display successfully!");
    }
}

void SqlStruct::insertTableEquipment(QLineEdit *id_echipament, QLineEdit *denumire, QLineEdit *nr_bucati, QLineEdit *pret_echipament, QLineEdit *data_equipment )
{
    QSqlQuery qry_insert(db_con);
        qry_insert.prepare("INSERT INTO Equipment ("+e[ID_Echipament]+", "+e[ID_Denumire]+", "+e[ID_Nr_bucati]+", "+e[ID_Pret_echipament]+",  "+e[ID_Data]+") "
                                                  "VALUES (:id, :n, :b, :p_e, :d)");
        qry_insert.bindValue(":id", id_echipament->text());
        qry_insert.bindValue(":n", denumire->text());
        qry_insert.bindValue(":b", nr_bucati->text());
        qry_insert.bindValue(":p_e", pret_echipament->text());
        qry_insert.bindValue(":d", data_equipment->text());

        if(qry_insert.exec())
        {
            qDebug() << "Inserted values.";
            QMessageBox::information(nullptr,"Army", "Values inserted!");
        }
        else
        {
            const QSqlError err = qry_insert.lastError();
                    if (err.type() == QSqlError::NoError) {
                        // Eroare neașteptată
                        qDebug() << "Error inserting values:" << err.text();
                        QMessageBox::critical(nullptr, "Army", "Error inserting values!");
                    } else {
                        // Eroare legată de bază de date
                        if (err.text().contains("UNIQUE constraint failed")) {
                            QMessageBox::information(nullptr, "Army", "There is already an equipment with this ID!");
                        } else {
                            qDebug() << "Error inserting values:" << err.text();
                            QMessageBox::critical(nullptr, "Army", "Error inserting values!");
                        }
                    }
        }
}


void SqlStruct::updateTableEquipment(QLineEdit *id_echipament, QLineEdit *denumire, QLineEdit *nr_bucati, QLineEdit *pret_echipament, QLineEdit *data_equipment) {
    QSqlQuery qry_update;

    bool updatePerformed = false; // Flag to track if any update was performed

    if (!denumire->text().isEmpty()) {
        QString updateQuery = "UPDATE Equipment SET " + e[ID_Denumire] + " = :noua_denumire WHERE " + e[ID_Echipament] + " = :id_echipament_de_actualizat";
        qry_update.prepare(updateQuery);
        qry_update.bindValue(":noua_denumire", denumire->text());
        qry_update.bindValue(":id_echipament_de_actualizat", id_echipament->text());

        if (!qry_update.exec()) {
            qDebug() << "Eroare la actualizare denumire:" << qry_update.lastError().text();
            return;
        }
        updatePerformed = true;
    }

    if (!nr_bucati->text().isEmpty()) {
        QString updateQuery = "UPDATE Equipment SET " + e[ID_Nr_bucati] + " = :noul_nr WHERE " + e[ID_Echipament] + " = :id_echipament_de_actualizat";
        qry_update.prepare(updateQuery);
        qry_update.bindValue(":noul_nr", nr_bucati->text());
        qry_update.bindValue(":id_echipament_de_actualizat", id_echipament->text());

        if (!qry_update.exec()) {
            qDebug() << "Eroare la actualizare nr_bucati:" << qry_update.lastError().text();
            return;
        }
        updatePerformed = true;
    }

    if (!pret_echipament->text().isEmpty()) {
        QString updateQuery = "UPDATE Equipment SET " + e[ID_Pret_echipament] + " = :nou_pret WHERE " + e[ID_Echipament] + " = :id_echipament_de_actualizat";
        qry_update.prepare(updateQuery);
        qry_update.bindValue(":nou_pret", pret_echipament->text());
        qry_update.bindValue(":id_echipament_de_actualizat", id_echipament->text());

        if (!qry_update.exec()) {
            qDebug() << "Eroare la actualizare pret_echipament:" << qry_update.lastError().text();
            return;
        }
        updatePerformed = true;
    }

    if (!data_equipment->text().isEmpty()) {
        QString updateQuery = "UPDATE Equipment SET " + e[ID_Data] + " = :noua_data WHERE " + e[ID_Echipament] + " = :id_echipament_de_actualizat";
        qry_update.prepare(updateQuery);
        qry_update.bindValue(":noua_data", data_equipment->text());
        qry_update.bindValue(":id_echipament_de_actualizat", id_echipament->text());

        if (!qry_update.exec()) {
            qDebug() << "Eroare la actualizare data:" << qry_update.lastError().text();
            return;
        }
        updatePerformed = true;
    }

    if (updatePerformed) {
        qDebug() << "Actualizare realizată cu succes!";
        QMessageBox::information(nullptr, "Army", "Update done successfully!");
    } else {
        qDebug() << "Nicio actualizare nu a fost realizată.";
        QMessageBox::information(nullptr, "Army", "No updates were performed.");
    }
}



void SqlStruct::deleteTableEquipment(QLineEdit *id_echipament, QLineEdit *denumire, QLineEdit *nr_bucati, QLineEdit *pret_echipament , QLineEdit *data_equipment)
{
    QSqlQuery qry_delete(db_con);

    ///delete id
    if(!id_echipament->text().isEmpty()){
        qry_delete.prepare("DELETE FROM Equipment WHERE "+e[ID_Echipament]+" = :ID;");
        qry_delete.bindValue(":ID", id_echipament->text());
    }
    ///delete denumire
    if(!denumire->text().isEmpty()){
        qry_delete.prepare("DELETE FROM Equipment WHERE "+e[ID_Denumire]+" = :d;");
        qry_delete.bindValue(":d", denumire->text());
    }
    ///delete nr_bucati && pret
    if(!nr_bucati->text().isEmpty() && !pret_echipament->text().isEmpty()){
        qry_delete.prepare("DELETE FROM Equipment WHERE "+e[ID_Nr_bucati]+" = :n AND "+e[ID_Pret_echipament]+" = :p");
        qry_delete.bindValue(":n", nr_bucati->text());
        qry_delete.bindValue(":p", pret_echipament->text());
    }
    ///delete data
    if(!data_equipment->text().isEmpty()){
        qry_delete.prepare("DELETE FROM Equipment WHERE "+e[ID_Data]+" = :d;");
        qry_delete.bindValue(":d", data_equipment->text());
    }

    if (!qry_delete.exec()) {
        QMessageBox::information(nullptr, "Army", "Error deleting record!");
        qDebug() <<  "Eroare la ștergerea înregistrării: " << qry_delete.lastError().text();
    } else {
        QMessageBox::information(nullptr, "Army", "Delete registration successfully completed!");
        qDebug() << "Stergere inregistrare realizată cu succes!";
    }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SqlStruct::getUsersContent(QTableView *tableView)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery *qry = new QSqlQuery(db_con);
    qry->prepare("SELECT * from Users");
    qry->exec();
    model->setQuery(*qry);
    tableView->setModel(model);
    qDebug() << (model->rowCount());

    if(!qry->exec())
    {
        qDebug() << "eroare afisare db" << qry->lastError();
        QMessageBox::information(nullptr,"Army", "Display error!");
    }else
    {
        qDebug() << "Afisare cu succes!";
        //QMessageBox::information(nullptr,"Army", "Display successfully!");
    }
}

void SqlStruct::insertTableUsers(QLineEdit *id_user, QLineEdit *userName, QLineEdit *password, QLineEdit *role)
{
    QSqlQuery qry_insert(db_con);
        qry_insert.prepare("INSERT INTO Users ("+u[ID_User]+", "+u[ID_UserName]+", "+u[ID_Password]+", "+u[ID_Role]+") "
                                                  "VALUES (:id, :u, :p, :r)");
        qry_insert.bindValue(":id", id_user->text());
        qry_insert.bindValue(":u", userName->text());
        qry_insert.bindValue(":p", password->text());
        qry_insert.bindValue(":r", role->text());

        if(qry_insert.exec())
        {
            qDebug() << "Inserted values.";
            QMessageBox::information(nullptr,"Army", "Values inserted!");
        }
        else
        {
            const QSqlError err = qry_insert.lastError();
                    if (err.type() == QSqlError::NoError) {
                        // Eroare neașteptată
                        qDebug() << "Error inserting values 1:" << err.text();
                        QMessageBox::critical(nullptr, "Army", "Error inserting values in table Users!");
                    } else {
                        // Eroare legată de bază de date
                        if (err.text().contains("UNIQUE constraint failed")) {
                            QMessageBox::information(nullptr, "Army", "There is already an equipment with this ID!");
                        } else {
                            qDebug() << "Error inserting values 2:" << err.text();
                            QMessageBox::critical(nullptr, "Army", "Error inserting values!");
                        }
                    }
        }
}

void SqlStruct::updateTableUsers(QLineEdit *id_user, QLineEdit *userName, QLineEdit *password, QLineEdit *role)
{
    QSqlQuery qry_update;

    if (!userName->text().isEmpty()) {
        QString updateQuery = "UPDATE Users SET " + u[ID_UserName] + " = :noul_userName WHERE " + u[ID_User] + " = :id_user_de_actualizat";
        qry_update.prepare(updateQuery);
        qry_update.bindValue(":noul_userName", userName->text());
        qry_update.bindValue(":id_user_de_actualizat", id_user->text());

        if (!qry_update.exec()) {
            qDebug() << "Eroare la actualizare userName:" << qry_update.lastError().text();
            return;
        }
    }

    if (!password->text().isEmpty()) {
        QString updateQuery = "UPDATE Users SET " + u[ID_Password] + " = :noua_parola WHERE " + u[ID_User] + " = :id_user_de_actualizat";
        qry_update.prepare(updateQuery);
        qry_update.bindValue(":noua_parola", password->text());
        qry_update.bindValue(":id_user_de_actualizat", id_user->text());

        if (!qry_update.exec()) {
            qDebug() << "Eroare la actualizare password:" << qry_update.lastError().text();
            return;
        }
    }

    if (!role->text().isEmpty()) {
        QString updateQuery = "UPDATE Users SET " + u[ID_Role] + " = :noul_rol WHERE " + u[ID_User] + " = :id_user_de_actualizat";
        qry_update.prepare(updateQuery);
        qry_update.bindValue(":noul_rol", role->text());
        qry_update.bindValue(":id_user_de_actualizat", id_user->text());

        if (!qry_update.exec()) {
            qDebug() << "Eroare la actualizare role:" << qry_update.lastError().text();
            return;
        }
    }

    if (!qry_update.exec()) {
        qDebug() << "Eroare la actualizare:" << qry_update.lastError().text();
        QMessageBox::information(nullptr, "Army", "Error updating!");
    } else {
        qDebug() << "Actualizare realizată cu succes!";
        QMessageBox::information(nullptr, "Army", "Update done successfully!");
    }
}

void SqlStruct::deleteTableUsers(QLineEdit *id_user, QLineEdit *userName, QLineEdit *password, QLineEdit *role)
{
    QSqlQuery qry_delete(db_con);

    ///delete id_user
    if(!id_user->text().isEmpty()){
        qry_delete.prepare("DELETE FROM Users WHERE "+u[ID_User]+" = :ID;");
        qry_delete.bindValue(":ID", id_user->text());
    }
    ///delete userName
    if(!userName->text().isEmpty()){
        qry_delete.prepare("DELETE FROM Users WHERE "+u[ID_UserName]+" = :u;");
        qry_delete.bindValue(":u", userName->text());
    }
    ///delete password && role
    if(!password->text().isEmpty() && !role->text().isEmpty()){
        qry_delete.prepare("DELETE FROM Users WHERE "+u[ID_Password]+" = :p AND "+u[ID_Role]+" = :r");
        qry_delete.bindValue(":p", password->text());
        qry_delete.bindValue(":r", role->text());
    }

    if (!qry_delete.exec()) {
        QMessageBox::information(nullptr, "Army", "Error deleting record!");
        qDebug() <<  "Eroare la ștergerea înregistrării: " << qry_delete.lastError().text();
    } else {
        QMessageBox::information(nullptr, "Army", "Delete registration successfully completed!");
        qDebug() << "Stergere inregistrare realizată cu succes!";
    }
}




