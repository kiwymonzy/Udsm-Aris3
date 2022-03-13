#include "databaseconnection.h"

DatabaseConnection::DatabaseConnection()
{
    this->setDatabaseName("ucc_db");
    this->setDatabaseDBFilePath("/sqlite/ucc_db.db");
}

/* Setter Methods */

void DatabaseConnection::setDatabaseName(QString databaseName){
    this->databaseName = databaseName;
}

void DatabaseConnection::setDatabaseDBFilePath(QString databaseDBFilePath){
    this->databaseDBFilePath = databaseDBFilePath;
}

/* Getter Methods */

QString DatabaseConnection::getDatabaseName(){
    return databaseName;
}

QString DatabaseConnection::getDatabaseDBFilePath(){
    return databaseDBFilePath;
}

/* Function Methods */
// Function to open database connection
bool DatabaseConnection::openDatebaseConnection(){

    // Assigning database configuration
    databaseConnection = QSqlDatabase::addDatabase("QSQLITE", this->databaseName);
    databaseConnection.setDatabaseName(this->databaseDBFilePath);

    // Establishing database connection and checking connection status
    if (!databaseConnection.open()){
        qWarning() << "Database Connection Error";
        qWarning() << databaseConnection.lastError();
        return false;
    }
    else{
        qWarning() << "Database Connection Open";
        return true;
    }

}

// Function to close database connection
void DatabaseConnection::closeDatebaseConnection(){

    // Closing established database connection
    databaseConnection.close();
    databaseConnection.removeDatabase(QSqlDatabase::defaultConnection);
    qWarning() << "Database Connection Closed";

}
