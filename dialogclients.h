#ifndef DIALOGCLIENTS_H
#define DIALOGCLIENTS_H

#include <QDialog>
#include "thirdparty/additional.h"
#include <QTableView>
#include <QTextStream>
#include <QTextCodec>
#include "dialogaddeditclient.h"

namespace Ui {
class DialogClients;
}

class DialogClients : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogClients(QWidget *parent = 0);
    void SetCurrentDbConnection(QSqlDatabase sdb);
    void getInitialClients();
    ~DialogClients();

public slots:
    void Process();
    void ShowDialogAddEditClient();
    void ShowDialogAddEditClient_Edit();
    void filterClients();
    void DeleteSelectedRowClient();
    
private:
    Ui::DialogClients *ui;
    QSqlDatabase current_sdb;
    //массив с названием полей в БД для фильтрации по клиентам
    QString filter_col_bd_names[10] = {"Id","Ur_name","Ur_adress","OGRN","Ur_checking_account","Agent_names","Agent_email","Agent_phone","Date_reg", "DateEdited"};


};

#endif // DIALOGCLIENTS_H
