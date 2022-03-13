#ifndef STUDENTWINDOW_H
#define STUDENTWINDOW_H

#include <QMainWindow>
#include "header.h"
#include <authenticatelogic.h>
#include <accountlogic.h>

namespace Ui {
class StudentWindow;
}

class StudentWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentWindow(int loginID, QWidget *parent = nullptr);
    ~StudentWindow();

private slots:
    void on_logout_pushButton_clicked();

private:
    Ui::StudentWindow *ui;
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

#endif // STUDENTWINDOW_H
