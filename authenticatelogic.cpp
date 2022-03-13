/***********************************************
**
**  Source Code Developed By: H.V.L.Hasanka
**
***********************************************/

#include "authenticatelogic.h"

// Public default constructor
AuthenticateLogic::AuthenticateLogic()
{

    // Creating an object of DatabaseConnection class
    trms_dbConnection = new DatabaseConnection();

}

/* Setter Methods */

void AuthenticateLogic::setLoginID(int loginID){
    this->loginID = loginID;
}

void AuthenticateLogic::setEmailAddress(QString emailAddress){
    this->emailAddress = emailAddress;
}

void AuthenticateLogic::setPasswordHash(QString passwordHash){
    this->passwordHash = passwordHash;
}

void AuthenticateLogic::setAccountStatusID(int accountStatusID){
    this->accountStatusID = accountStatusID;
}

void AuthenticateLogic::setAccountStatus(QString accountStatus){
    this->accountStatus = accountStatus;
}

void AuthenticateLogic::setAccountTypeID(int accountTypeID){
    this->accountTypeID = accountTypeID;
}

void AuthenticateLogic::setAccountType(QString accountType){
    this->accountType = accountType;
}

void AuthenticateLogic::setAccountActivityID(int accountActivityID){
    this->accountActivityID = accountActivityID;
}

void AuthenticateLogic::setAccountActivity(QString accountActivity){
    this->accountActivity = accountActivity;
}

void AuthenticateLogic::setLoginActivityID(int loginActivityID){
    this->loginActivityID = loginActivityID;
}


/* Getter Methods */
int AuthenticateLogic::getLoginID(){
    return loginID;
}

QString AuthenticateLogic::getEmailAddress(){
    return emailAddress;
}

QString AuthenticateLogic::getPasswordHash(){
    return passwordHash;
}

int AuthenticateLogic::getAccountStatusID(){
    return accountStatusID;
}

QString AuthenticateLogic::getAccountStatus(){
    return accountStatus;
}

int AuthenticateLogic::getAccountTypeID(){
    return accountTypeID;
}

QString AuthenticateLogic::getAccountType(){
    return accountType;
}

int AuthenticateLogic::getAccountActivityID(){
    return accountActivityID;
}

QString AuthenticateLogic::getAccountActivity(){
    return accountActivity;
}

int AuthenticateLogic::getLoginActivityID(){
    return loginActivityID;
}


/* Function Methods */

QString AuthenticateLogic::retrieveCurrentDateTime(){
    /* Getting the current datetime timestamp from the localhost */
    QDateTime currentDateTimeTimeStamp = QDateTime::currentDateTime();
    // Reformatting datetime timestamp
    QString currentDateTime = currentDateTimeTimeStamp.toString("yyyy-MM-dd hh:mm:ss");
    return currentDateTime;
}

bool AuthenticateLogic::validateEnteredEmailAddress(QString enteredEmailAddressValue){
    // Checking whether the entered email address is under the required regular expression
    QRegularExpression re("^[A-Za-z0-9_]+@[a-zA-Z_]+?.[a-zA-Z]{2,3}+");
    QRegularExpressionMatch validationCheck = re.match(enteredEmailAddressValue);
    bool validationResponse = validationCheck.hasMatch();
    return validationResponse;
}

bool AuthenticateLogic::validateEnteredPassword(QString enteredPasswordValue){
    // Checking whether the entered password is under the required regular expression
    // Password Guidelines:
    //  Minimum of seven (7) characters
    //  Maximum of twenty (20) characters
    //  Atleast one uppercase letter
    //  Atleast one lowercase letter
    //  Atleast one numeric character
    //  Atleast one special character
    QRegularExpression re("^(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[!@#[\\$%[\\^&[\\*])(?=.{7,20})");
    QRegularExpressionMatch validationCheck = re.match(enteredPasswordValue);
    bool validationResponse = validationCheck.hasMatch();
    return validationResponse;
}

string AuthenticateLogic::generatePasswordHash(string passwordValue){
    // Declaring a new object
    hash<string> generateHash;
    // Generating the hash value from the passed passwordValue
    // (generateHash(passwordValue)) - returns a long value, this will be converted to a string
    std::string hashValue = std::to_string((generateHash(passwordValue)));
    // Returning the generated hash value to the called command
    return hashValue;
}

QString AuthenticateLogic::checkEmailAddressAvailability(QString enteredEmailAddress){
    bool databaseConnection = trms_dbConnection->openDatebaseConnection();
    if(databaseConnection == true){

        // Declaring new QSqlQuery object by passing the database name
        QSqlQuery emailAddressAvailabilityQuery(QSqlDatabase::database(trms_dbConnection->getDatabaseName()));

        // Preparing sql query for execution
        emailAddressAvailabilityQuery.prepare(QString("SELECT * FROM Login WHERE EmailAddress=:enteredEmailAddress;"));

        emailAddressAvailabilityQuery.bindValue(":enteredEmailAddress", enteredEmailAddress);

        // Executing sql query and checking the status
        if(!emailAddressAvailabilityQuery.exec()){
            qWarning() << "SQL query execution error";
            qWarning() << emailAddressAvailabilityQuery.lastError();
            trms_dbConnection->closeDatebaseConnection();
            return "Execution Unsuccessful: SQL query execution error";
        }
        else{
            if(emailAddressAvailabilityQuery.next()){
                trms_dbConnection->closeDatebaseConnection();
                return "Account Available with Entered Email Address";
             }
            else{
                trms_dbConnection->closeDatebaseConnection();
                return "Account Unavailable with Entered Email Address";
            }
        }

    }
    else if(databaseConnection == false){
        qWarning() << "Database Connection Error";
        trms_dbConnection->closeDatebaseConnection();
        return "Execution Unsuccessful: Database Connection Error";
    }
    trms_dbConnection->closeDatebaseConnection();
    return "default";
}

QString AuthenticateLogic::registerNewUser(QString selectedNamePrefix, QString enteredFirstName,
                                           QString enteredMiddleName, QString enteredLastName,
                                           QString enteredEmailAddress, QString generatedConfirmPasswordHash,
                                           QString selectedAccountType){

    bool databaseConnection = trms_dbConnection->openDatebaseConnection();
    if(databaseConnection == true){

        int successfulQueryExecutions = 0;

        /* Inserting record into the 'Login' relation (table)  */
        // Declaring new QSqlQuery object by passing the database name
        QSqlQuery loginQuery(QSqlDatabase::database(trms_dbConnection->getDatabaseName()));

        // Preparing sql query for execution
        loginQuery.prepare(QString("INSERT INTO Login(EmailAddress, PasswordHash, asAccountStatusID) VALUES "
                                              "(:enteredEmailAddress, :generatedConfirmPasswordHash, 1);"));

        loginQuery.bindValue(":enteredEmailAddress", enteredEmailAddress);
        loginQuery.bindValue(":generatedConfirmPasswordHash", generatedConfirmPasswordHash);

        // Executing sql query and checking the status
        if(!loginQuery.exec()){
            qWarning() << "SQL query execution error";
            qWarning() << loginQuery.lastError();
            trms_dbConnection->closeDatebaseConnection();
            return "Execution Unsuccessful: SQL query execution error";
        }
        else{
            successfulQueryExecutions++;qWarning() << "1";
        }

        /* Retrieving the 'LoginID' from the 'Login' relation (table) of the newly added record */
        QString loginIDDB;
        // Declaring new QSqlQuery object by passing the database name
        QSqlQuery loginIDQuery(QSqlDatabase::database(trms_dbConnection->getDatabaseName()));

        // Preparing sql query for execution
        loginIDQuery.prepare(QString("SELECT LoginID FROM Login WHERE EmailAddress=:enteredEmailAddress;"));

        loginIDQuery.bindValue(":enteredEmailAddress", enteredEmailAddress);

        // Executing sql query and checking the status
        if(!loginIDQuery.exec()){
            qWarning() << "SQL query execution error";
            qWarning() << loginIDQuery.lastError();
            trms_dbConnection->closeDatebaseConnection();
            return "Execution Unsuccessful: SQL query execution error";
        }
        else{
            if(loginIDQuery.next()){
                loginIDDB = loginIDQuery.value(0).toString();
            }
            successfulQueryExecutions++;qWarning() << "2";
        }


        /* Retrieving the 'NamePrefixID' from the 'NamePrefix' relation (table) for the user selected name prefix */
        QString namePrefixIDDB;
        // Declaring new QSqlQuery object by passing the database name
        QSqlQuery namePrefixIDDBQuery(QSqlDatabase::database(trms_dbConnection->getDatabaseName()));

        // Preparing sql query for execution
        namePrefixIDDBQuery.prepare(QString("SELECT NamePrefixID FROM NamePrefix WHERE NamePrefix = :selectedNamePrefix"));

        namePrefixIDDBQuery.bindValue(":selectedNamePrefix", selectedNamePrefix);

        // Executing sql query and checking the status
        if(!namePrefixIDDBQuery.exec()){
            qWarning() << "SQL query execution error";
            qWarning() << namePrefixIDDBQuery.lastError();
            trms_dbConnection->closeDatebaseConnection();
            return "Execution Unsuccessful: SQL query execution error";
        }
        else{
            if(namePrefixIDDBQuery.next()){
                namePrefixIDDB = namePrefixIDDBQuery.value(0).toString();
            }
            successfulQueryExecutions++;qWarning() << "3";
        }


        /* Retrieving the 'AccountTypeID' from the 'AccountType' relation (table) for the user selected account type */
        QString accountTypeIDDB;
        // Declaring new QSqlQuery object by passing the database name
        QSqlQuery accountTypeIDDBQuery(QSqlDatabase::database(trms_dbConnection->getDatabaseName()));

        // Preparing sql query for execution
        accountTypeIDDBQuery.prepare(QString("SELECT AccountTypeID FROM AccountType WHERE AccountType=:selectedAccountType"));

        accountTypeIDDBQuery.bindValue(":selectedAccountType", selectedAccountType);

        // Executing sql query and checking the status
        if(!accountTypeIDDBQuery.exec()){
            qWarning() << "SQL query execution error";
            qWarning() << accountTypeIDDBQuery.lastError();
            trms_dbConnection->closeDatebaseConnection();
            return "Execution Unsuccessful: SQL query execution error";
        }
        else{
            if(accountTypeIDDBQuery.next()){
                accountTypeIDDB = accountTypeIDDBQuery.value(0).toString();
            }
            successfulQueryExecutions++;qWarning() << "4";
        }


        /* Inserting record into the 'Account' relation (table)  */
        // Declaring new QSqlQuery object by passing the database name
        QSqlQuery accountQuery(QSqlDatabase::database(trms_dbConnection->getDatabaseName()));

        // Preparing sql query for execution
        accountQuery.prepare(QString("INSERT INTO Account(FirstName, MiddleName, LastName, CreatedDateTime, npNamePrefixID, "
                                     "atAccountTypeID, dvDoNotDistrubBooleanValueID, lLoginID) VALUES "
                                     "(:enteredFirstName, :enteredMiddleName, :enteredLastName, :createdDateTime, :selectedNamePrefixID, "
                                     ":selectedAccountTypeID, 2, :generatedLoginID);"));

        accountQuery.bindValue(":enteredFirstName", enteredFirstName);
        accountQuery.bindValue(":enteredMiddleName", enteredMiddleName);
        accountQuery.bindValue(":enteredLastName", enteredLastName);
        accountQuery.bindValue(":createdDateTime", this->retrieveCurrentDateTime());
        accountQuery.bindValue(":selectedNamePrefixID", namePrefixIDDB.toInt());
        accountQuery.bindValue(":selectedAccountTypeID", accountTypeIDDB.toInt());
        accountQuery.bindValue(":generatedLoginID", loginIDDB.toInt());

        // Executing sql query and checking the status
        if(!accountQuery.exec()){
            qWarning() << "SQL query execution error";
            qWarning() << accountQuery.lastError();
            trms_dbConnection->closeDatebaseConnection();
            return "Execution Unsuccessful: SQL query execution error";
        }
        else{
            successfulQueryExecutions++;qWarning() << "5";
        }


        /* Retrieving the AccountID of the newly registered user from the Acount relation (table) */
        QString accountIDDB;
        // Declaring new QSqlQuery object by passing the database name
        QSqlQuery accountIDDBQuery(QSqlDatabase::database(trms_dbConnection->getDatabaseName()));

        // Preparing sql query for execution
        accountIDDBQuery.prepare(QString("SELECT a.AccountID FROM "
                                         "Account a INNER JOIN Login l ON l.LoginID = a.lLoginID "
                                         "WHERE l.EmailAddress = :emailAddress;"));

        accountIDDBQuery.bindValue(":emailAddress", enteredEmailAddress);

        // Executing sql query and checking the status
        if(!accountIDDBQuery.exec()){
            qWarning() << "SQL query execution error";
            qWarning() << accountIDDBQuery.lastError();
            trms_dbConnection->closeDatebaseConnection();
            return "Execution Unsuccessful: SQL query execution error";
        }
        else{
            if(accountIDDBQuery.next()){
                accountIDDB = accountIDDBQuery.value(0).toString();
            }
            successfulQueryExecutions++;qWarning() << "6";
        }


        /* Each user has only four types of tasks */
        /* These types contains details that can be edited by the user */
        /* Creating four new category types by inserting fours new records into Category relation (table) */

        int categoryNameNumber = 1;
        int categoryTypeID = 1;

        // Loop iterating four times to insert four records
        for(int i = 0; i < 4; i++){

            // Declaring new QSqlQuery object by passing the database name
            QSqlQuery categoryQuery(QSqlDatabase::database(trms_dbConnection->getDatabaseName()));

            // Preparing sql query for execution
            categoryQuery.prepare(QString("INSERT INTO Category(CategoryName, ctCategoryTypeID, ccColourID, aAccountID) VALUES "
                                          "(:categoryName, :categoryTypeID, 1, :accountID);"));

            categoryQuery.bindValue(":categoryName", "Category " + QString::number(categoryNameNumber));
            categoryQuery.bindValue(":categoryTypeID", categoryTypeID);
            categoryQuery.bindValue(":accountID", accountIDDB.toInt());

            // Executing sql query and checking the status
            if(!categoryQuery.exec()){
                qWarning() << "SQL query execution error";
                qWarning() << categoryQuery.lastError();
                trms_dbConnection->closeDatebaseConnection();
                return "Execution Unsuccessful: SQL query execution error";
            }
            else{
                successfulQueryExecutions++;qWarning() << "7";
            }

            // Incrementing categoryNameNumber and categoryTypeID by one in each iteration
            categoryNameNumber++;
            categoryTypeID++;

        }


        // Checking the number successful executions
        if(successfulQueryExecutions == 10){
            trms_dbConnection->closeDatebaseConnection();
            return "New Account Successfully Created";
        }
        else{
            trms_dbConnection->closeDatebaseConnection();
            qWarning() << "New Account Creation Error";
            return "New Account Creation Error";
        }

    }
    else if(databaseConnection == false){
        qWarning() << "Database Connection Error";
        trms_dbConnection->closeDatebaseConnection();
        return "Execution Unsuccessful: Database Connection Error";
    }

    trms_dbConnection->closeDatebaseConnection();
    return "default";
}

QString AuthenticateLogic::loginCredentialVerification(QString enteredEmailAddress, QString enteredPasswordHash){

    /* Retrieving user's data from the database if the an account is available with the entered email address */

    // Opening database connection
    bool databaseConnection = trms_dbConnection->openDatebaseConnection();

    if(databaseConnection == true){
        // Declaring new QSqlQuery object by passing the database name
        QSqlQuery vertificationQuery(QSqlDatabase::database(trms_dbConnection->getDatabaseName()));

        // Preparing sql query for execution
        vertificationQuery.prepare(QString("SELECT l.loginID, l.PasswordHash, acts.AccountStatusID, acts.AccountStatus, "
                              "at.AccountTypeID, at.AccountType FROM "
                              "Login l INNER JOIN Account a ON l.LoginID = a.lLoginID "
                              "INNER JOIN AccountStatus acts ON acts.AccountStatusID = l.asAccountStatusID "
                              "INNER JOIN AccountType at ON at.AccountTypeID = a.atAccountTypeID "
                              "WHERE EmailAddress=:enteredEmailAddress;"));

        vertificationQuery.bindValue(":enteredEmailAddress", enteredEmailAddress);

        // Executing sql query and checking the status
        if(!vertificationQuery.exec()){
            qWarning() << "SQL query execution error";
            qWarning() << vertificationQuery.lastError();
            trms_dbConnection->closeDatebaseConnection();
            return "Verification Unsuccessful: SQL query execution error";
        }
        else{
            if(vertificationQuery.next()){
                this->setLoginID(vertificationQuery.value(0).toInt());
                this->setPasswordHash(vertificationQuery.value(1).toString());
                this->setAccountStatusID(vertificationQuery.value(2).toInt());
                this->setAccountStatus(vertificationQuery.value(3).toString());
                this->setAccountTypeID(vertificationQuery.value(4).toInt());
                this->setAccountType(vertificationQuery.value(5).toString());
            }
            else{
                trms_dbConnection->closeDatebaseConnection();
                return "Verification Unsuccessful: No Account Available with Entered Email Address";
            }
        }
    }
    else{
        qWarning() << "Database Connection Error";
        trms_dbConnection->closeDatebaseConnection();
        return "Verification Unsuccessful: Database Connection Error";
    }

    // Closing database connection
    trms_dbConnection->closeDatebaseConnection();

    /* Checking whether the password hash value match */
    if(this->getPasswordHash() == enteredPasswordHash){

        /* Checking account status of the user */
        if(this->getAccountStatus() == "Active"){

            /* Checking account type of the user */
            if(this->getAccountType() == "Employee User Account"){
                trms_dbConnection->closeDatebaseConnection();
                return "Verification Successful: Account Type: UserAccount";
            }
            else if(this->getAccountType() == "Student User Account"){
                trms_dbConnection->closeDatebaseConnection();
                return "Verification Successful: Account Type: UserAccount";
            }
            else if(this->getAccountType() == "Admin Account"){
                trms_dbConnection->closeDatebaseConnection();
                return "Verification Successful: Account Type: AdminAccount";
            }
        }
        else if(this->getAccountStatus() == "Disabled"){
            trms_dbConnection->closeDatebaseConnection();
            return "Verification Unsuccessful: Account Disabled";
        }

    }
    else if(this->getPasswordHash() != enteredPasswordHash){
        trms_dbConnection->closeDatebaseConnection();
        return "Verification Unsuccessful: Password Incorrect";
    }
    trms_dbConnection->closeDatebaseConnection();
    return "default";
}

QString AuthenticateLogic::addSessionStartToDB(){

    this->setAccountActivityID(1);
    this->setAccountActivity("Online");

    bool databaseConnected = trms_dbConnection->openDatebaseConnection();
    if(databaseConnected == true){

        /* Inserting new record into 'LoginActivity' relation (table) */
        // Declaring new QSqlQuery object by passing the database name
        QSqlQuery sessionStartQuery(QSqlDatabase::database(trms_dbConnection->getDatabaseName()));

        // Preparing sql query for execution
        sessionStartQuery.prepare(QString("INSERT INTO LoginActivity(LoginDateTime, LogoutDateTime, OperatingSystemSpec, aaAccountActivityID, lLoginID) VALUES "
                                          "(:loginDateTime, '-', '-', :accountActivityID, :loginID);"));

        sessionStartQuery.bindValue(":loginDateTime", this->retrieveCurrentDateTime());
        sessionStartQuery.bindValue(":accountActivityID", this->getAccountActivityID());
        sessionStartQuery.bindValue(":loginID", this->getLoginID());

        // Executing sql query and checking the status
        if(!sessionStartQuery.exec()){
            qWarning() << "SQL query execution error";
            qWarning() << sessionStartQuery.lastError();
            trms_dbConnection->closeDatebaseConnection();
            return "SQL Execution Failed";
        }
        else{

            /* Retrieving 'LoginActivityID' for the newly inserted record */
            // Declaring new QSqlQuery object by passing the database name
            QSqlQuery loginActivityIDQuery(QSqlDatabase::database(trms_dbConnection->getDatabaseName()));

            // Preparing sql query for execution
            loginActivityIDQuery.prepare(QString("SELECT LoginActivityID FROM LoginActivity WHERE lLoginID = :loginID "
                                                 "ORDER BY LoginDateTime DESC LIMIT 1;"));

            loginActivityIDQuery.bindValue(":loginID", this->getLoginID());

            // Executing sql query and checking the status
            if(!loginActivityIDQuery.exec()){
                qWarning() << "SQL query execution error";
                qWarning() << loginActivityIDQuery.lastError();
                trms_dbConnection->closeDatebaseConnection();
                return "SQL Execution Failed";
            }
            else{
                this->setLoginActivityID(loginActivityIDQuery.value(0).toInt());
                trms_dbConnection->closeDatebaseConnection();
                qWarning() << "Session Start Recorded";
                return "Session Start Recorded";
            }
        }

    }
    else{
        qWarning() << "Database Connection Error";
        trms_dbConnection->closeDatebaseConnection();
        return "Database Connectivity Failed";
    }

    trms_dbConnection->closeDatebaseConnection();

}

QString AuthenticateLogic::addSessionEndToDB(int loginID){

    this->setAccountActivityID(2);
    this->setAccountActivity("Offline");

    // Assigning database configuration
    databaseConnection = QSqlDatabase::addDatabase("QSQLITE", "ucc_db");
    databaseConnection.setDatabaseName("/sqlite/ucc_db.db");

    // Establishing database connection and checking connection status
    if (!databaseConnection.open()){
        qWarning() << "Database Connection Error";
        // Closing established database connection
        databaseConnection.close();
        databaseConnection.removeDatabase(QSqlDatabase::defaultConnection);
        qWarning() << "Database Connection Closed";
        return "Database Connectivity Failed";
    }
    else{
        int loginActivityID = 0;

        // Declaring new QSqlQuery object by passing the database name
        QSqlQuery LoginActivityIDQuery(QSqlDatabase::database(trms_dbConnection->getDatabaseName()));

        // Preparing sql query for execution
        LoginActivityIDQuery.prepare(QString("SELECT LoginActivityID FROM LoginActivity WHERE lLoginID = :loginID ORDER BY LoginDateTime DESC LIMIT 1;"));

        LoginActivityIDQuery.bindValue(":loginID", loginID);

        // Executing sql query and checking the status
        if(!LoginActivityIDQuery.exec()){
            qWarning() << "SQL query execution error";
            qWarning() << LoginActivityIDQuery.lastError();
            // Closing established database connection
            databaseConnection.close();
            databaseConnection.removeDatabase(QSqlDatabase::defaultConnection);
            qWarning() << "Database Connection Closed";
        }
        else{
            if(LoginActivityIDQuery.next()){
                loginActivityID = LoginActivityIDQuery.value(0).toInt();
            }
        }

        // Declaring new QSqlQuery object by passing the database name
        QSqlQuery sessionEndQuery(QSqlDatabase::database(trms_dbConnection->getDatabaseName()));

        // Preparing sql query for execution
        sessionEndQuery.prepare(QString("UPDATE LoginActivity SET LogoutDateTime = :logoutDateTime, aaAccountActivityID = :accountActivityID "
                                        "WHERE LoginActivityID = :loginActivityID;"));

        sessionEndQuery.bindValue(":logoutDateTime", this->retrieveCurrentDateTime());
        sessionEndQuery.bindValue(":accountActivityID", this->getAccountActivityID());
        sessionEndQuery.bindValue(":loginActivityID", loginActivityID);

        // Executing sql query and checking the status
        if(!sessionEndQuery.exec()){
            qWarning() << "SQL query execution error";
            qWarning() << sessionEndQuery.lastError();
            // Closing established database connection
            databaseConnection.close();
            databaseConnection.removeDatabase(QSqlDatabase::defaultConnection);
            qWarning() << "Database Connection Closed";
            return "SQL Execution Failed";
        }
        else{
            qWarning() << "Session End Recorded";
            // Closing established database connection
            databaseConnection.close();
            databaseConnection.removeDatabase(QSqlDatabase::defaultConnection);
            qWarning() << "Database Connection Closed";
            return "Session End Recorded";
        }
    }

    // Closing established database connection
    databaseConnection.close();
    databaseConnection.removeDatabase(QSqlDatabase::defaultConnection);
    qWarning() << "Database Connection Closed";
    return "default";
}
