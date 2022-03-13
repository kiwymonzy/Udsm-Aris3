/***********************************************
**
**  Source Code Developed By: Kiwy Monzy
**
***********************************************/

#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QMessageBox>

#include <authenticatelogic.h>
#include <studentwindow.h>
#include <adminaccountwindow.h>


#include <header.h>
//#include <userregistrationwindow.h>
//#include <reportsubmissionwindow.h>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class QSystemTrayIcon;

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    // Public variable declaration
    bool enteredEmailAddressValueAcceptable;
    bool enteredPasswordValueAcceptable;

private slots:
    void on_showPassword_checkBox_stateChanged(int arg1);
    void on_emailAddress_lineEdit_textChanged(const QString &arg1);
    void on_password_lineEdit_textChanged(const QString &arg1);
    void on_login_pushButton_clicked();


    void on_logout_pushButton_clicked();

private:
    Ui::LoginWindow *ui;
    QLineEdit *password_lineEdit;
    // Connecting the authenticate.h
    // Retireving the authenticate.h class path to a pointer
    AuthenticateLogic *auth;
    // Connecting the receptionwindow.h
    // Retireving the receptionwindow.h class path to a pointer
    StudentWindow *StudentWindowForm;
    // Connecting the adminaccountwindow.h
    // Retireving the adminaccountwindow.h class path to a pointer
    AdminAccountWindow *adminAccountWindowForm;
    // Connecting the userregistrationwindow.h
    // Retireving the userregistrationwindow.h class path to a pointer
 //   UserRegistrationWindow *userRegistrationWindowForm;
    // Connecting the reportsubmissionwindow.h
    // Retireving the reportsubmissionwindow.h class path to a pointer
 //   ReportSubmissionWindow *reportSubmissionWindowFrom;


};
#endif // LOGINWINDOW_H
