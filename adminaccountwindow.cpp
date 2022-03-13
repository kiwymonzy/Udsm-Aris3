#include "adminaccountwindow.h"
#include "ui_adminaccountwindow.h"

AdminAccountWindow::AdminAccountWindow(int loginID, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminAccountWindow)
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
   ui->stackedWidget_admin_tabs->setCurrentIndex(0);

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
        QSqlQuery adminDetailsQuery(QSqlDatabase::database(trms_dbConnection->getDatabaseName()));

        // Preparing sql query for execution
        adminDetailsQuery.prepare(QString("SELECT l.LoginID, a.AccountID, np.NamePrefix, a.FirstName, a.MiddleName, a.LastName, "
                                         "a.CreatedDateTime, at.AccountTypeID, at.AccountType FROM "
                                         "Login l INNER JOIN Account a ON l.LoginID = a.lLoginID "
                                         "INNER JOIN NamePrefix np ON np.NamePrefixID = a.npNamePrefixID "
                                         "INNER JOIN AccountType at ON at.AccountTypeID = a.atAccountTypeID "
                                         "WHERE LoginID = :loginID;"));

        adminDetailsQuery.bindValue(":loginID", loginID);

        // Executing sql query and checking the status
        if(!adminDetailsQuery.exec()){
            qWarning() << "SQL query execution error";
            qWarning() << adminDetailsQuery.lastError();
            trms_dbConnection->closeDatebaseConnection();
        }
        else{
            if(adminDetailsQuery.next()){
                account->setLoginID(adminDetailsQuery.value(0).toInt());
                account->setAccountID(adminDetailsQuery.value(1).toInt());
                account->setNamePrefix(adminDetailsQuery.value(2).toString());
                account->setFirstName(adminDetailsQuery.value(3).toString());
                account->setMiddleName(adminDetailsQuery.value(4).toString());
                account->setLastName(adminDetailsQuery.value(5).toString());
                account->setCreatedDateTime(adminDetailsQuery.value(6).toString());
                account->setAccountTypeID(adminDetailsQuery.value(7).toInt());
                account->setAccountType(adminDetailsQuery.value(8).toString());
            }
            trms_dbConnection->closeDatebaseConnection();
        }

        /* Retrieving the relevant user details from the database and assigning it to the user interface */
        /* Getting number of standard user accounts from the database */
        // Declaring new QSqlQuery object by passing the database name
        QSqlQuery noOfStandardAccountQuery(QSqlDatabase::database(trms_dbConnection->getDatabaseName()));

        // Preparing sql query for execution
        noOfStandardAccountQuery.prepare(QString("SELECT COUNT(AccountID) FROM Account WHERE atAccountTypeID = 2;"));

        // Executing sql query and checking the status
        if(!noOfStandardAccountQuery.exec()){
            qWarning() << "SQL query execution error";
            qWarning() << noOfStandardAccountQuery.lastError();
        }
        else{
            if(noOfStandardAccountQuery.next()){
                ui->noOfStudentAccounts_label->setText(noOfStandardAccountQuery.value(0).toString());
            }
        }

        /* Retrieving the relevant user details from the database and assigning it to the user interface */
        /* Getting number of premium user accounts from the database */
        // Declaring new QSqlQuery object by passing the database name
        QSqlQuery noOfPremiumAccountQuery(QSqlDatabase::database(trms_dbConnection->getDatabaseName()));

        // Preparing sql query for execution
        noOfPremiumAccountQuery.prepare(QString("SELECT COUNT(AccountID) FROM Account WHERE atAccountTypeID = 1;"));

        // Executing sql query and checking the status
        if(!noOfPremiumAccountQuery.exec()){
            qWarning() << "SQL query execution error";
            qWarning() << noOfPremiumAccountQuery.lastError();
        }
        else{
            if(noOfPremiumAccountQuery.next()){
                ui->noOfEmployeeAccounts_label->setText(noOfPremiumAccountQuery.value(0).toString());
            }
        }

        trms_dbConnection->closeDatebaseConnection();
    }
    else if(databaseConnected == false){
        qWarning() << "Database Connection Error";
        trms_dbConnection->closeDatebaseConnection();
    }




    /* REPORTS TAB */

    // Setting 'noNewReportsAvailable_label' to hide
    ui->noNewReportsAvailable_label->setVisible(false);

    // Setting 'feedbackReport_tableView' to hide
//    ui->feedbackReport_tableView->setVisible(false);

    bool databaseConnectedCategory = trms_dbConnection->openDatebaseConnection();
    if(databaseConnectedCategory == true){

        // Declaring new QSqlQuery object by passing the database name
        QSqlQuery reportQuery(QSqlDatabase::database(trms_dbConnection->getDatabaseName()));

        // Preparing sql query for execution
        reportQuery.prepare(QString("SELECT r.ReportID, r.ReportSubject, r.Feedback FROM "
                                    "Report r INNER JOIN ReviewStatus ts ON ts.ReviewStatusID = r.rsReviewStatusID "
                                    "WHERE ts.ReviewStatus = 'Review Pending';"));

        // Executing sql query and checking the status
        if(!reportQuery.exec()){
            qWarning() << "SQL query execution error";
            qWarning() << reportQuery.lastError();
        }
        else{
            if(reportQuery.next()){
                QSqlQueryModel *reportModal = new QSqlQueryModel();
                reportModal->setQuery(reportQuery);

                ui->feedbackReport_tableView->setVisible(true);
                ui->feedbackReport_tableView->setModel(reportModal);
                ui->feedbackReport_tableView->resizeColumnsToContents();
            }
            else{
                ui->noNewReportsAvailable_label->setVisible(true);
            }
        }
        trms_dbConnection->closeDatebaseConnection();
    }
    else if(databaseConnectedCategory == false){
        trms_dbConnection->closeDatebaseConnection();
        qWarning() << "Database Connection Error";
    }


    /* PROFILE TAB */

    /* Profile Image Implementation Source Code */
    // Getting the profile image
//    QPixmap profileImagePix(":/images/ProfileImage.png");
    // Getting the width of the profileImage_label
//    int profileLabelWidth = ui->profileImage_label->width();
    // Getting the height of the profileImage_label
//    int profileLabelHeight = ui->profileImage_label->height();
    // Setting the profile image of the profileImage_label
//    ui->profileImage_label->setPixmap(profileImagePix.scaled(profileLabelWidth, profileLabelHeight, Qt::KeepAspectRatio));

    /* Assigning user data into the user inferface */
    ui->nameText_label->setText(account->getNamePrefix() + " " + account->getFirstName() + "  " + account->getMiddleName() + " " + account->getLastName());
    ui->profileText_label->setText(account->getNamePrefix() + "" + account->getFirstName() + " " + account->getLastName());
    ui->accountTypeText_label->setText(account->getAccountType());
    ui->accountCreatedDateTimeText_label->setText(account->getCreatedDateTime());

    bool databaseConnectedEmailAddress = trms_dbConnection->openDatebaseConnection();
    if(databaseConnectedEmailAddress == true){

        // Getting the user email address from the database
        QSqlQuery emailAddressQuery(QSqlDatabase::database(trms_dbConnection->getDatabaseName()));

        // Preparing sql query for execution
        emailAddressQuery.prepare(QString("SELECT l.EmailAddress FROM "
                                          "Login l INNER JOIN Account a ON l.loginID = a.lLoginID "
                                          "WHERE a.AccountID = :accountID;"));

        emailAddressQuery.bindValue(":accountID", account->getAccountID());

        // Executing sql query and checking the status
        if(!emailAddressQuery.exec()){
            qWarning() << "SQL query execution error";
            qWarning() << emailAddressQuery.lastError();
        }
        else{
            while(emailAddressQuery.next()){
                ui->emailAddressText_label->setText(emailAddressQuery.value(0).toString());
            }
        }

        /* Retrieving the account activity details from the database */
        // Declaring new QSqlQuery object by passing the database name
        QSqlQuery accountActivityQuery(QSqlDatabase::database(trms_dbConnection->getDatabaseName()));

        // Preparing sql query for execution
        accountActivityQuery.prepare(QString("SELECT la.LoginDateTime as 'Login Date Time', la.LogoutDateTime as 'Logout Date Time', aa.AccountActivity as 'Account Activity' FROM "
                                             "LoginActivity la INNER JOIN Login l ON l.LoginID = la.lLoginID "
                                             "INNER JOIN AccountActivity aa ON aa.AccountActivityID = la.aaAccountActivityID "
                                             "INNER JOIN Account a ON l.LoginID = a.lLoginID "
                                             "WHERE a.AccountID = :accountID ORDER BY la.LoginDateTime DESC;"));

        accountActivityQuery.bindValue(":accountID", account->getAccountID());

        // Executing sql query and checking the status
        if(!accountActivityQuery.exec()){
            qWarning() << "SQL query execution error";
            trms_dbConnection->closeDatebaseConnection();
            qWarning() << accountActivityQuery.lastError();
        }
        else{
            if(accountActivityQuery.next()){
                QSqlQueryModel *accountActivityModel = new QSqlQueryModel();
                accountActivityModel->setQuery(accountActivityQuery);

                ui->accountActivity_tableView->setModel(accountActivityModel);
                ui->accountActivity_tableView->resizeColumnsToContents();
            }

            trms_dbConnection->closeDatebaseConnection();
        }

    }
    else if(databaseConnectedEmailAddress == false){
        qWarning() << "Database Connectivity ERROR";
    }



    /* SETTINGS TAB */

    /* About Image Implementation Source Code */
    // Getting the about image
    QPixmap aboutImagePix(":/images/AboutImage.png");
    // Getting the width of the aboutImage_label
//    int aboutLabelWidth = ui->aboutImage_label->width();
    // Getting the height of the aboutImage_label
//    int aboutLabelHeight = ui->aboutImage_label->height();
    // Setting the cover image of the aboutImage_label
//    ui->aboutImage_label->setPixmap(aboutImagePix.scaled(aboutLabelWidth, aboutLabelHeight, Qt::KeepAspectRatio));


}

AdminAccountWindow::~AdminAccountWindow()
{
    delete ui;
}

void AdminAccountWindow::on_tabWidget_currentChanged(int index)
{
    if(index == 0){
        /* DASHBOARD TAB */

        /* Retrieving the relevant user details from the database and assigning it to the user interface */
        bool databaseConnected = trms_dbConnection->openDatebaseConnection();
        if(databaseConnected == true){

            /* Getting number of standard user accounts from the database */
            // Declaring new QSqlQuery object by passing the database name
            QSqlQuery noOfStandardAccountQuery(QSqlDatabase::database(trms_dbConnection->getDatabaseName()));

            // Preparing sql query for execution
            noOfStandardAccountQuery.prepare(QString("SELECT COUNT(AccountID) FROM Account WHERE atAccountTypeID = 1;"));

            // Executing sql query and checking the status
            if(!noOfStandardAccountQuery.exec()){
                qWarning() << "SQL query execution error";
                qWarning() << noOfStandardAccountQuery.lastError();
            }
            else{
                if(noOfStandardAccountQuery.next()){
                    ui->noOfStudentAccounts_label->setText(noOfStandardAccountQuery.value(0).toString());
                }
            }

            /* Getting number of premium user accounts from the database */
            // Declaring new QSqlQuery object by passing the database name
            QSqlQuery noOfPremiumAccountQuery(QSqlDatabase::database(trms_dbConnection->getDatabaseName()));

            // Preparing sql query for execution
            noOfPremiumAccountQuery.prepare(QString("SELECT COUNT(AccountID) FROM Account WHERE atAccountTypeID = 1;"));

            // Executing sql query and checking the status
            if(!noOfPremiumAccountQuery.exec()){
                qWarning() << "SQL query execution error";
                qWarning() << noOfPremiumAccountQuery.lastError();
            }
            else{
                if(noOfPremiumAccountQuery.next()){
                    ui->noOfEmployeeAccounts_label->setText(noOfStandardAccountQuery.value(0).toString());
                }
            }

            trms_dbConnection->closeDatebaseConnection();
        }
        else if(databaseConnected == false){
            qWarning() << "Database Connection Error";
            trms_dbConnection->closeDatebaseConnection();
        }

    }

}

void AdminAccountWindow::on_showMoreDetails_pushButton_clicked()
{

    /* Passing user entered 'ReportID' */
    /* Opening viewfeedbackreportdialog to show more report details */
//    viewFeedbackReportDialogForm = new ViewFeedbackReportDialog(ui->showMoreDetails_lineEdit->text().toInt(), this);
//    viewFeedbackReportDialogForm->show();

}

void AdminAccountWindow::on_logout_pushButton_clicked()
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

void AdminAccountWindow::on_actionAbout_triggered()
{

//    aboutDialogForm = new AboutDialog(this);
//    aboutDialogForm->show();

}

void AdminAccountWindow::on_actionLogout_triggered()
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

void AdminAccountWindow::on_actionExit_triggered()
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


//TABS
void AdminAccountWindow::on_pushButton_dashboard_clicked()
{
    ui->stackedWidget_admin_tabs->setCurrentIndex(0);
}


void AdminAccountWindow::on_pushButton_student_clicked()
{
    ui->stackedWidget_admin_tabs->setCurrentIndex(1);
}


void AdminAccountWindow::on_pushButton_report_clicked()
{
    ui->stackedWidget_admin_tabs->setCurrentIndex(2);
}


void AdminAccountWindow::on_register_pushButton_clicked()
{

    QString selectedNamePrefix = ui->namePrefix_comboBox->currentText();
    QString enteredFirstName = ui->firstName_lineEdit->text();
    QString enteredMiddleName = ui->middleName_lineEdit->text();
    QString enteredLastName = ui->lastName_lineEdit->text();
    QString enteredEmailAddress = ui->emailAddress_lineEdit->text();
    QString enteredPassword = ui->password_lineEdit->text();
    QString enteredConfirmPassword = ui->confirmPassword_lineEdit->text();
    QString selectedAccountType = ui->accountType_comboBox->currentText();

    // Getting the string length of the middleName text, to check whether user has entered any characters
    int enteredMiddleNameStringLength = ui->middleName_lineEdit->text().length();

    // If the user has not entered any characters
    if(enteredMiddleNameStringLength == 0){

        if(enteredFirstNameValueAcceptable == true && enteredLastNameValueAcceptable == true
                && enteredEmailAddressValueAcceptable == true && enteredPasswordValueAcceptable == true
                && enteredConfirmPasswordValueAcceptable == true && checkedTermsAndConditions == true){

            // Checking whether the entered password and confirm passwords are matching
            if(enteredPassword == enteredConfirmPassword){

                // Checking the availability of the entered email address
                QString emailAddressAvailablilityStatus = auth->checkEmailAddressAvailability(enteredEmailAddress);

                if(emailAddressAvailablilityStatus == "Account Unavailable with Entered Email Address"){

                    // Generating password hash of the entered confirm password
                    // Generating hash value of entered confirm password value
                    QString generatedPasswordHash = QString::fromStdString(auth->generatePasswordHash(enteredConfirmPassword.toStdString()));

                    QString userRegistrationStatus = auth->registerNewUser(selectedNamePrefix,
                                                                           enteredFirstName,
                                                                           "",
                                                                           enteredLastName,
                                                                           enteredEmailAddress,
                                                                           generatedPasswordHash,
                                                                           selectedAccountType);

                    if(userRegistrationStatus == "New Account Successfully Created"){
                        QMessageBox::information(this, "NEW ACCOUNT REGISTERED", "Please entered your login credentials to login.");
                        this->hide();
                    }
                    else if(userRegistrationStatus == "New Account Creation Error"){
                        QMessageBox::critical(this, "ACCOUNT REGISTRATION ERROR", "Please submit a report and try again later.\nApologies for the inconvenience.");
                    }
                    else if(userRegistrationStatus == "Execution Unsuccessful: SQL query execution error"){
                        QMessageBox::critical(this, "ACCOUNT REGISTRATION ERROR", "Please submit a report and try again later.\nApologies for the inconvenience.");
                    }
                    else if(userRegistrationStatus == "Execution Unsuccessful: Database Connection Error"){
                        QMessageBox::critical(this, "ACCOUNT REGISTRATION ERROR", "Please submit a report and try again later.\nApologies for the inconvenience.");
                    }
                }
                else if(emailAddressAvailablilityStatus == "Account Available with Entered Email Address"){
                    QMessageBox::critical(this, "REGISTRATION ERROR", "Account with the entered email address is already available, please enter another email address");
                }
                else if(emailAddressAvailablilityStatus == "Execution Unsuccessful: SQL query execution error"){
                    QMessageBox::critical(this, "REGISTRATION ERROR", "SQL query execution error, please submit a report.\nApologies for the inconvenience.");
                }
                else if(emailAddressAvailablilityStatus == "Execution Unsuccessful: Database Connection Error"){
                    QMessageBox::critical(this, "REGISTRATION ERROR", "Unable to connect to the database, please submit a report.\nApologies for the inconvenience.");
                }
            }
            else{
                QMessageBox::critical(this, "REGISTRATION ERROR", "Entered passwords do not match, please re-enter.");
            }
        }
        else{
            qWarning() << "Relavant fields does not contain acceptable values";
        }
    }
    else{
    // If the user has entered any characters

        if(enteredFirstNameValueAcceptable == true && enteredMiddleNameValueAcceptable == true
                && enteredLastNameValueAcceptable == true && enteredEmailAddressValueAcceptable == true
                && enteredPasswordValueAcceptable == true && enteredConfirmPasswordValueAcceptable == true
                && checkedTermsAndConditions == true){

            // Checking whether the entered password and confirm passwords are matching
            if(enteredPassword == enteredConfirmPassword){

                // Checking the availability of the entered email address
                QString emailAddressAvailablilityStatus = auth->checkEmailAddressAvailability(enteredEmailAddress);

                if(emailAddressAvailablilityStatus == "Account Unavailable with Entered Email Address"){

                    // Generating password hash of the entered confirm password
                    // Generating hash value of entered confirm password value
                    QString generatedPasswordHash = QString::fromStdString(auth->generatePasswordHash(enteredConfirmPassword.toStdString()));



                    QString userRegistrationStatus = auth->registerNewUser(selectedNamePrefix,
                                                                           enteredFirstName,
                                                                           enteredMiddleName,
                                                                           enteredLastName,
                                                                           enteredEmailAddress,
                                                                           generatedPasswordHash,
                                                                           selectedAccountType);

                    if(userRegistrationStatus == "New Account Successfully Created"){
                        QMessageBox::information(this, "NEW ACCOUNT REGISTERED", "Successfully Registered. Please entered your login credentials to login.");
                        this->hide();
                    }
                    else if(userRegistrationStatus == "New Account Creation Error"){
                        QMessageBox::critical(this, "ACCOUNT REGISTRATION ERROR", "Please submit a report and try again later.\nApologies for the inconvenience.");
                    }
                    else if(userRegistrationStatus == "Execution Unsuccessful: SQL query execution error"){
                        QMessageBox::critical(this, "ACCOUNT REGISTRATION ERROR", "Please submit a report and try again later.\nApologies for the inconvenience.");
                    }
                    else if(userRegistrationStatus == "Execution Unsuccessful: Database Connection Error"){
                        QMessageBox::critical(this, "ACCOUNT REGISTRATION ERROR", "Please submit a report and try again later.\nApologies for the inconvenience.");
                    }
                }
                else if(emailAddressAvailablilityStatus == "Account Available with Entered Email Address"){
                    QMessageBox::critical(this, "REGISTRATION ERROR", "Account with the entered email address is already available, please enter another email address");
                }
                else if(emailAddressAvailablilityStatus == "Execution Unsuccessful: SQL query execution error"){
                    QMessageBox::critical(this, "REGISTRATION ERROR", "SQL query execution error, please submit a report.\nApologies for the inconvenience.");
                }
                else if(emailAddressAvailablilityStatus == "Execution Unsuccessful: Database Connection Error"){
                    QMessageBox::critical(this, "REGISTRATION ERROR", "Unable to connect to the database, please submit a report.\nApologies for the inconvenience.");
                }
            }
            else{
                QMessageBox::critical(this, "REGISTRATION ERROR", "Entered passwords do not match, please re-enter.");
            }
        }
        else{
            qWarning() << "Relavant fields does not contain acceptable values";
        }

    }

}


void AdminAccountWindow::on_termsAndConditions_checkBox_stateChanged(int arg1)
{
    if(arg1){
        checkedTermsAndConditions = true;
    }
    else{
        checkedTermsAndConditions = false;
    }

    if(enteredFirstNameValueAcceptable == true && enteredLastNameValueAcceptable == true
            && enteredEmailAddressValueAcceptable == true && enteredPasswordValueAcceptable == true
            && enteredConfirmPasswordValueAcceptable == true && checkedTermsAndConditions == true){
        // Enabling register push button
        ui->register_pushButton->setEnabled(true);
    }
    else{
        // Disabling register push button
        ui->register_pushButton->setEnabled(false);
    }
}


void AdminAccountWindow::on_confirmPassword_lineEdit_textChanged(const QString &arg1)
{
    // Checking whether the user entered confirm password value is in the correct regular expression
    // arg1 = ui->confirmPassword_lineEdit->text();
    QString enteredConfirmPassword = arg1;
    QRegularExpression re("^(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[!@#[\\$%[\\^&[\\*])(?=.{7,20})");
    QRegularExpressionMatch validationCheck = re.match(enteredConfirmPassword);
    bool validationResponse = validationCheck.hasMatch();
    if(validationResponse == true){
        // Changing lineEdit border styles
        ui->confirmPassword_lineEdit->setStyleSheet("border: 2px solid green;"
                                                 "background-color: rgb(255, 255, 255);");
        // Setting enteredConfirmPasswordValueAcceptable to true
        enteredConfirmPasswordValueAcceptable = true;
    }
    else if (validationResponse == false){
        // Changing lineEdit border styles
        ui->confirmPassword_lineEdit->setStyleSheet("border: 2px solid red;"
                                                 "background-color: rgb(255, 255, 255);");
        // Setting enteredConfirmPasswordValueAcceptable to false as the entered confirm password value is not acceptable
        enteredConfirmPasswordValueAcceptable = false;
    }

    if(enteredFirstNameValueAcceptable == true && enteredLastNameValueAcceptable == true
            && enteredEmailAddressValueAcceptable == true && enteredPasswordValueAcceptable == true
            && enteredConfirmPasswordValueAcceptable == true && checkedTermsAndConditions == true){
        // Enabling register push button
        ui->register_pushButton->setEnabled(true);
    }
    else{
        // Disabling register push button
        ui->register_pushButton->setEnabled(false);
    }
}



void AdminAccountWindow::on_password_lineEdit_textChanged(const QString &arg1)
{
    {
        // Checking whether the user entered password value is in the correct regular expression
        // arg1 = ui->password_lineEdit->text();
        QString enteredPassword = arg1;
        QRegularExpression re("^(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[!@#[\\$%[\\^&[\\*])(?=.{7,20})");
        QRegularExpressionMatch validationCheck = re.match(enteredPassword);
        bool validationResponse = validationCheck.hasMatch();
        if(validationResponse == true){
            // Changing lineEdit border styles
            ui->password_lineEdit->setStyleSheet("border: 2px solid green;"
                                                     "background-color: rgb(255, 255, 255);");
            // Setting enteredPasswordValueAcceptable to true
            enteredPasswordValueAcceptable = true;
        }
        else if (validationResponse == false){
            // Changing lineEdit border styles
            ui->password_lineEdit->setStyleSheet("border: 2px solid red;"
                                                     "background-color: rgb(255, 255, 255);");
            // Setting enteredPasswordValueAcceptable to false as the entered password value is not acceptable
            enteredPasswordValueAcceptable = false;
        }

        if(enteredFirstNameValueAcceptable == true && enteredLastNameValueAcceptable == true
                && enteredEmailAddressValueAcceptable == true && enteredPasswordValueAcceptable == true
                && enteredConfirmPasswordValueAcceptable == true && checkedTermsAndConditions == true){
            // Enabling register push button
            ui->register_pushButton->setEnabled(true);
        }
        else{
            // Disabling register push button
            ui->register_pushButton->setEnabled(false);
        }
    }
}

// When the user changes the 'emailAddress' text
void AdminAccountWindow::on_emailAddress_lineEdit_textChanged(const QString &arg1)
{
    // Checking whether the user entered email address value is in the correct regular expression
    // arg1 = ui->emailAddress_lineEdit->text();
    QString enteredEmailAddress = arg1;
    QRegularExpression re("^[A-Za-z0-9_]+@[a-zA-Z_]+?.[a-zA-Z]{2,3}+");
    QRegularExpressionMatch validationCheck = re.match(enteredEmailAddress);
    bool validationResponse = validationCheck.hasMatch();
    if(validationResponse == true){
        // Changing lineEdit border styles
        ui->emailAddress_lineEdit->setStyleSheet("border: 2px solid green;"
                                                 "background-color: rgb(255, 255, 255);");
        // Setting enteredEmailAddressValueAcceptable to true
        enteredEmailAddressValueAcceptable = true;
    }
    else if (validationResponse == false){
        // Changing lineEdit border styles
        ui->emailAddress_lineEdit->setStyleSheet("border: 2px solid red;"
                                                 "background-color: rgb(255, 255, 255);");
        // Setting enteredEmailAddressValueAcceptable to false as the entered email address value is not acceptable
        enteredEmailAddressValueAcceptable = false;
    }

    if(enteredFirstNameValueAcceptable == true && enteredLastNameValueAcceptable == true
            && enteredEmailAddressValueAcceptable == true && enteredPasswordValueAcceptable == true
            && enteredConfirmPasswordValueAcceptable == true && checkedTermsAndConditions == true){
        // Enabling register push button
        ui->register_pushButton->setEnabled(true);
    }
    else{
        // Disabling register push button
        ui->register_pushButton->setEnabled(false);
    }
}

// When user changes the 'lastName' text
void AdminAccountWindow::on_lastName_lineEdit_textChanged(const QString &arg1)
{
    // Checking whether the user entered last name value is in the correct regular expression
    // arg1 = ui->lastName_lineEdit->text();
    QString enteredLastName = arg1;
    QRegularExpression re("^[A-z]+$");
    QRegularExpressionMatch validationCheck = re.match(enteredLastName);
    bool validationResponse = validationCheck.hasMatch();
    if(validationResponse == true){
        // Changing lineEdit border styles
        ui->lastName_lineEdit->setStyleSheet("border: 2px solid green;"
                                                 "background-color: rgb(255, 255, 255);");
        // Setting enteredLastNameValueAcceptable to true
        enteredLastNameValueAcceptable = true;
    }
    else if (validationResponse == false){
        // Changing lineEdit border styles
        ui->lastName_lineEdit->setStyleSheet("border: 2px solid red;"
                                                 "background-color: rgb(255, 255, 255);");
        // Setting enteredLastNameValueAcceptable to false as the entered last name value is not acceptable
        enteredLastNameValueAcceptable = false;
    }

    if(enteredFirstNameValueAcceptable == true && enteredLastNameValueAcceptable == true
            && enteredEmailAddressValueAcceptable == true && enteredPasswordValueAcceptable == true
            && enteredConfirmPasswordValueAcceptable == true && checkedTermsAndConditions == true){
        // Enabling register push button
        ui->register_pushButton->setEnabled(true);
    }
    else{
        // Disabling register push button
        ui->register_pushButton->setEnabled(false);
    }
}

// When user changes the 'middleName' text
void AdminAccountWindow::on_middleName_lineEdit_textChanged(const QString &arg1)
{
    // Checking whether the user entered middle name value is in the correct regular expression
    // arg1 = ui->middleName_lineEdit->text();
    QString enteredMiddleName = arg1;
    QRegularExpression re("^[A-z]+$");
    QRegularExpressionMatch validationCheck = re.match(enteredMiddleName);
    bool validationResponse = validationCheck.hasMatch();
    if(validationResponse == true){
        // Changing lineEdit border styles
        ui->middleName_lineEdit->setStyleSheet("border: 2px solid green;"
                                                 "background-color: rgb(255, 255, 255);");
        // Setting enteredMiddleNameValueAcceptable to true
        enteredMiddleNameValueAcceptable = true;
    }
    else if (validationResponse == false){
        // Changing lineEdit border styles
        ui->middleName_lineEdit->setStyleSheet("border: 2px solid red;"
                                                 "background-color: rgb(255, 255, 255);");
        // Setting enteredMiddleNameValueAcceptable to false as the entered middle name value is not acceptable
        enteredMiddleNameValueAcceptable = false;
    }
}

// When user changes the 'firstName' text
void AdminAccountWindow::on_firstName_lineEdit_textChanged(const QString &arg1)
{
    // Checking whether the user entered first name value is in the correct regular expression
    // arg1 = ui->firstName_lineEdit->text();
    QString enteredFirstName = arg1;
    QRegularExpression re("^[A-z]+$");
    QRegularExpressionMatch validationCheck = re.match(enteredFirstName);
    bool validationResponse = validationCheck.hasMatch();
    if(validationResponse == true){
        // Changing lineEdit border styles
        ui->firstName_lineEdit->setStyleSheet("border: 2px solid green;"
                                                 "background-color: rgb(255, 255, 255);");
        // Setting enteredFirstNameValueAcceptable to true
        enteredFirstNameValueAcceptable = true;
    }
    else if (validationResponse == false){
        // Changing lineEdit border styles
        ui->firstName_lineEdit->setStyleSheet("border: 2px solid red;"
                                                 "background-color: rgb(255, 255, 255);");
        // Setting enteredFirstNameValueAcceptable to false as the entered first name value is not acceptable
        enteredFirstNameValueAcceptable = false;
    }

    if(enteredFirstNameValueAcceptable == true && enteredLastNameValueAcceptable == true
            && enteredEmailAddressValueAcceptable == true && enteredPasswordValueAcceptable == true
            && enteredConfirmPasswordValueAcceptable == true && checkedTermsAndConditions == true){
        // Enabling register push button
        ui->register_pushButton->setEnabled(true);
    }
    else{
        // Disabling register push button
        ui->register_pushButton->setEnabled(false);
    }
}
