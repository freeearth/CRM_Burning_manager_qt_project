#ifndef DIALOGADDEDITORDER_H
#define DIALOGADDEDITORDER_H

#include <QDialog>
#include "thirdparty/additional.h"
#include <QRegExpValidator>

namespace Ui {
class DialogAddEditOrder;
}

class DialogAddEditOrder : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogAddEditOrder(QWidget *parent = 0);
    bool SetComboBoxClientsId(QStringList Ur_names,QStringList Id);
    bool SetOldOrderInformation();
    QString Ur_Id;
    QString Ur_Name;
    QString Order_summ;
    QString Order_status;
    int ModalResult = -1;
    QString ConfirmationText;
    ~DialogAddEditOrder();

public slots:
    void Process();
    void Exit();
    void ConfirmationBtnClick();
    
private:
    Ui::DialogAddEditOrder *ui;
};

#endif // DIALOGADDEDITORDER_H
