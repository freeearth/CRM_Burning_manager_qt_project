#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTextStream>
#include <QTextCodec>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>

#include <QtSql>
#include <QTableView>
#include <QImage>

#include "thirdparty/additional.h"
#include "dialogclients.h"
#include "dialogaddeditorder.h"
#include "dialogabout.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void ShowMessage(QString message);
    void ConnectToDB();
    bool fileExists(QString path);
    ~MainWindow();

public slots:
    void Process();
    void getInitiaOrders();
    void filterOrders();
    void ShowClientsDictionary();
    void AddNewOrder();
    void DeleteSelectedOrder();
    void EditSelectedOrder();
    void ShowAboutDlg();
    void LoadFunImage();
    
private:
    Ui::MainWindow *ui;
    QSqlDatabase current_sdb;
    QString db_name = "TestManagerCRM.db";//имя БД
   //массив с названием полей в БД для фильтрации в заказах
    QString filter_col_bd_names[9] = {"a.Order_id","Order_sum","b.Ur_name","b.Ur_adress","b.Agent_names","b.Agent_email","b.Agent_phone","a.Order_date","a.Edited_datetime"};
};

#endif // MAINWINDOW_H
