#include "studentwindow.h"
#include "ui_studentwindow.h"

StudentWindow::StudentWindow(int loginID, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StudentWindow)
{
    ui->setupUi(this);

    /* Logo Image Implementation Source Code */
    // Getting the logo image
//    QPixmap logoImagePix(":/images/TRMS-Logo-Without-Text.jpg");
    // Getting the width of the logo_label
//    int logoLabelWidth = ui->logo_label->width();
    // Getting the height of the logo_label
//    int logoLabelHeight = ui->logo_label->height();
    // Setting the cover image of the logo_label
//    ui->logo_label->setPixmap(logoImagePix.scaled(logoLabelWidth, logoLabelHeight, Qt::KeepAspectRatio));

    // Setting 'Dashboard' tab as the initial tab
   ui->stackedWidget_dashboard_tabs->setCurrentIndex(0);

    // Creating an object of DatabaseConnection class
    trms_dbConnection = new DatabaseConnection();

    // Creating an object of AuthenticateLogic class
    auth = new AuthenticateLogic();

    // Creating an object of AccountLogic class
    account = new AccountLogic();


    /* DASHBOARD TAB */

    bool databaseConnected = trms_dbConnection->openDatebaseConnection();
    if(databaseConnected == true){

        // Declaring new QSqlQuery object by passing the database name
        QSqlQuery studentDetailsQuery(QSqlDatabase::database(trms_dbConnection->getDatabaseName()));

        // Preparing sql query for execution
        studentDetailsQuery.prepare(QString("SELECT l.LoginID, a.AccountID, np.NamePrefix, a.FirstName, a.MiddleName, a.LastName, "
                                         "a.CreatedDateTime, at.AccountTypeID, at.AccountType FROM "
                                         "Login l INNER JOIN Account a ON l.LoginID = a.lLoginID "
                                         "INNER JOIN NamePrefix np ON np.NamePrefixID = a.npNamePrefixID "
                                         "INNER JOIN AccountType at ON at.AccountTypeID = a.atAccountTypeID "
                                         "WHERE LoginID = :loginID;"));

        studentDetailsQuery.bindValue(":loginID", loginID);

        // Executing sql query and checking the status
        if(!studentDetailsQuery.exec()){
            qWarning() << "SQL query execution error";
            qWarning() << studentDetailsQuery.lastError();
            trms_dbConnection->closeDatebaseConnection();
        }
        else{
            if(studentDetailsQuery.next()){
                account->setLoginID(studentDetailsQuery.value(0).toInt());
                account->setAccountID(studentDetailsQuery.value(1).toInt());
                account->setNamePrefix(studentDetailsQuery.value(2).toString());
                account->setFirstName(studentDetailsQuery.value(3).toString());
                account->setMiddleName(studentDetailsQuery.value(4).toString());
                account->setLastName(studentDetailsQuery.value(5).toString());
                account->setCreatedDateTime(studentDetailsQuery.value(6).toString());
                account->setAccountTypeID(studentDetailsQuery.value(7).toInt());
                account->setAccountType(studentDetailsQuery.value(8).toString());
            }
            trms_dbConnection->closeDatebaseConnection();
        }
        trms_dbConnection->closeDatebaseConnection();
    }
    else if(databaseConnected == false){
        qWarning() << "Database Connection Error";
        trms_dbConnection->closeDatebaseConnection();
    }

    /* Assigning user data into the user inferface */
    ui->profileText_label->setText(account->getNamePrefix() + "" + account->getFirstName() + " " + account->getLastName());
    ui->accountTypeText_label->setText(account->getAccountType());
}

StudentWindow::~StudentWindow()
{
    delete ui;
}

void StudentWindow::on_logout_pushButton_clicked()
{

    // Showing message box to the user to get the confirmation to logout
    int userLogoutResponse = QMessageBox::question(this, "LOGOUT CONFIRMATION",
                          "This action will logout you out, do you want to continue?"
                          , "Cancel", "Continue");

    if(userLogoutResponse == 1){
        /* Recording session end */
        QString sessionEndStatus = auth->addSessionEndToDB(account->getLoginID());

        if(sessionEndStatus == "Session End Recorded"){
            this->hide();

        }
        else if(sessionEndStatus == "SQL Execution Failed"){
            QMessageBox::critical(this, "LOGIN - SESSION END ERROR", "SQL query execution was unsuccessful, please submit a report including your email address.");
        }
        else if(sessionEndStatus == "Database Connectivity Failed"){
            QMessageBox::critical(this, "LOGIN - SESSION END ERROR", "Database Connection has lost, please submit a report.");
        }

    }
}
