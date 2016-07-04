#ifndef DIALOGADDEDITCLIENT_H
#define DIALOGADDEDITCLIENT_H

#include <QDialog>
#include "thirdparty/additional.h"
#include <QRegExpValidator>

namespace Ui {
class DialogAddEditClient;
}

class DialogAddEditClient : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogAddEditClient(QWidget *parent = 0);
    bool SetOldClientInformation();
    QString Ur_Name;
    QString Ur_Adress;
    QString OGRN;
    QString Ur_Checking_account;
    QString Agent_FIO;
    QString Agent_Email;
    QString Agent_Phone;
    QString ConfirmationText;
    int Modalresult = -1;
    ~DialogAddEditClient();

public slots:
    void Process();
    void Exit();
    void ConfirmationBtnClick();



private:
    Ui::DialogAddEditClient *ui;
};

#endif // DIALOGADDEDITCLIENT_H
