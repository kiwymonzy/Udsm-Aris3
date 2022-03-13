#ifndef ADMINACCOUNTWINDOW_H
#define ADMINACCOUNTWINDOW_H

#include <QMainWindow>

#include "databaseconnection.h"
//#include "viewfeedbackreportdialog.h"
#include "authenticatelogic.h"
//#include "aboutdialog.h"
#include "accountlogic.h"
#include "header.h"

namespace Ui {
class AdminAccountWindow;
}

class AdminAccountWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminAccountWindow(int loginID, QWidget *parent = nullptr);
    ~AdminAccountWindow();
    // Public variable declaration
    bool enteredFirstNameValueAcceptable;
    bool enteredMiddleNameValueAcceptable;
    bool enteredLastNameValueAcceptable;
    bool enteredEmailAddressValueAcceptable;
    bool enteredPasswordValueAcceptable;
    bool enteredConfirmPasswordValueAcceptable;
    bool checkedTermsAndConditions;
    bool selectedAccountType;

private slots:
    void on_tabWidget_currentChanged(int index);

    void on_showMoreDetails_pushButton_clicked();

    void on_logout_pushButton_clicked();

    void on_actionAbout_triggered();

    void on_actionLogout_triggered();

    void on_actionExit_triggered();


    void on_pushButton_dashboard_clicked();

    void on_pushButton_student_clicked();

    void on_pushButton_report_clicked();

    void on_register_pushButton_clicked();

    void on_termsAndConditions_checkBox_stateChanged(int arg1);

    void on_confirmPassword_lineEdit_textChanged(const QString &arg1);

    void on_password_lineEdit_textChanged(const QString &arg1);

    void on_firstName_lineEdit_textChanged(const QString &arg1);

    void on_lastName_lineEdit_textChanged(const QString &arg1);

    void on_emailAddress_lineEdit_textChanged(const QString &arg1);

    void on_middleName_lineEdit_textChanged(const QString &arg1);

private:
    Ui::AdminAccountWindow *ui;
    // Connecting the databaseconnecion.h
    // Retireving the databaseconnecion.h class path to a pointer
    DatabaseConnection *trms_dbConnection;
    // Connecting the viewfeedbackreportdialog.h
    // Retireving the viewfeedbackreportdialog.h class path to a pointer
    //    ViewFeedbackReportDialog *viewFeedbackReportDialogForm;
    // Connecting the authenticate.h
    // Retireving the authenticate.h class path to a pointer
    AuthenticateLogic *auth;
    // Connecting the aboutdialog.h
    // Retireving the aboutdialog.h class path to a pointer
    //    AboutDialog *aboutDialogForm;
    // Connecting the accountlogic.h
    // Retireving the accountlogic.h class path to a pointer
    AccountLogic *account;
    //Showing login window after logout


};

#endif // ADMINACCOUNTWINDOW_H
