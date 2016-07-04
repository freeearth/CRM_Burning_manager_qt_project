#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //для правильного отображения кириллицы
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("CP-1251"));
    setWindowIcon(QIcon("stranger.png"));
    ui->setupUi(this);

    //массив с названием полей для фильтрации в заказах
    QString filter_col_names[9] = {"Номер заказа","Сумма заказа","Наименование юр.лица","Адрес юр.лица","ФИО агента","Email агента","Телефон агента", "Дата и время создания\nYYYY-MM-DD hh:mm:ss", "Дата и время редактирования\nYYYY-MM-DD hh:mm:ss"};

    for (int i = 0; i < sizeof(filter_col_names)/sizeof(QString);i++) {
        ui->comboBoxFilterOrders->addItem(filter_col_names[i], this->filter_col_bd_names[i]);
    }
    this->ConnectToDB();
    this->getInitiaOrders();
}

bool MainWindow::fileExists(QString path) {
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    return check_file.exists() && check_file.isFile();
}

void MainWindow::Process() {
    //связать кнопку с окном, для вывода сообщения, при нажатии
    //connect(ui->BtnTestSqlite, SIGNAL(clicked()), SLOT(getInitiaOrders()) );
    connect(ui->lineEditFiltering,SIGNAL(textChanged(QString)),SLOT(filterOrders()));
    connect(ui->BtnClientsDictionary,SIGNAL(clicked()),SLOT(ShowClientsDictionary()));

    connect(ui->BtnAddNewOrder,SIGNAL(clicked()),SLOT(AddNewOrder()));
    connect(ui->BtnDeleteSelectedOrder,SIGNAL(clicked()),SLOT(DeleteSelectedOrder()));
    connect(ui->BtnEditSelectedOrder,SIGNAL(clicked()),SLOT(EditSelectedOrder()));
    connect(ui->actionAboutUs,SIGNAL(triggered()),SLOT(ShowAboutDlg()));
    connect(ui->actionFunImage,SIGNAL(triggered()),SLOT(LoadFunImage()));
}

/*
 *Подключаемсяк БД
 *
 */
void MainWindow::ConnectToDB() {
    this->current_sdb = QSqlDatabase::addDatabase("QSQLITE");
    this->current_sdb.setDatabaseName(this->db_name);
    QDir dir;
    if (!this->fileExists(dir.absolutePath()+"/TestManagerCRM.db")) {
        Additional::ShowMessage("Ошибка подключения к базе данных!");
        return;
    }
    else {
        bool db_ok = this->current_sdb.open();
        if (!db_ok) {
            Additional::ShowMessage("Ошибка подключения к базе данных!");
            return;
        }
    }
}

/*
 *
 *Получить начальный набор заказов без фильтрации
 *
 */
void MainWindow::getInitiaOrders() {
            /*
             *Пример получения всех данных из одной таблицы в tableView
            QSqlTableModel *model = new QSqlTableModel(0, sdb);
            model->setTable("Orders_");
            model->select();
            ui->tableView->setModel(model);
            */

            /*
             *объединённый запрос отображаем в таблице
             */
            QSqlQueryModel *model_query = new QSqlQueryModel();
            QString queryString = "SELECT a.Order_id,a.Total_sum as Order_sum,CASE a.Payment_status WHEN 1 THEN 'Оплачен' ELSE 'Не оплачен' END as Status,"
                    "a.Order_date,a.Edited_datetime,b.Ur_name || '('||b.Id||')',b.Ur_adress,b.OGRN,b.Ur_checking_account,b.Agent_names,b.Agent_email,b.Agent_phone, b.Id as ClientID"
                    " FROM `Orders_` as `a` LEFT JOIN `Clients_` as `b` ON a.Client_id == b.id";
            model_query->setQuery(queryString, this->current_sdb);

            Additional *Add = new Additional();
            Add->GetLastQueryError(model_query);

            Additional::SetModelHeaderOrders(model_query);
            ui->TableViewOrders->setModel(model_query);
            ui->TableViewOrders->horizontalHeader()->setStretchLastSection(true);
            ui->TableViewOrders->setColumnHidden(12, true);
}

/*
 *Фильтрация заказов
 *
 *
 */
void MainWindow::filterOrders() {
    try {
        QString FilterValue =ui->lineEditFiltering->text().trimmed();
        QString FilterDbColumn =ui->comboBoxFilterOrders->itemData(ui->comboBoxFilterOrders->currentIndex()).toString();
        QSqlQueryModel *model_query = new QSqlQueryModel();
        QString queryString = "SELECT a.Order_id,a.Total_sum as Order_sum,CASE a.Payment_status WHEN 1 THEN 'Оплачен' ELSE 'Не оплачен' END as Status,"
                "a.Order_date,a.Edited_datetime,b.Ur_name || '('||b.Id||')',b.Ur_adress,b.OGRN,b.Ur_checking_account,b.Agent_names,b.Agent_email,b.Agent_phone, b.Id as ClientID"
                " FROM `Orders_` as `a` LEFT JOIN `Clients_` as `b` ON a.Client_id == b.id WHERE " + FilterDbColumn + " LIKE '%"+FilterValue+"%'";
        model_query->setQuery(queryString, this->current_sdb);
        Additional *Add = new Additional();
        Add->GetLastQueryError(model_query);

        Additional::SetModelHeaderOrders(model_query);
        ui->TableViewOrders->setModel(model_query);
        ui->TableViewOrders->setColumnHidden(12, true);
    }
    catch(...) {
        Additional::ShowMessage("Ошибка фильтрации!");
    }
}

/*
 *Показать словарь "Клиенты"
 */
void MainWindow::ShowClientsDictionary() {
    DialogClients *ClientsDictionary = new DialogClients();
    ClientsDictionary->SetCurrentDbConnection(this->current_sdb);
    ClientsDictionary->exec();
    this->filterOrders();//применим фильтрацию
    return;
}

/*
 *Добавить новый заказ
 */
void MainWindow::AddNewOrder() {
    try {
        /*
         *Запрос всех клиентов
         */
        QSqlQuery *query = new QSqlQuery();
        query->exec("SELECT Id, Ur_name FROM Clients_");
        Additional *Add = new Additional();
        Add->GetLastQueryError(query);
        int size = 0;
        while (query->next()) {
            size++;
        }
        if(size>0) {
            DialogAddEditOrder *AddNewOrder = new DialogAddEditOrder();
            AddNewOrder->ConfirmationText = "Будет добавлен новый заказ?";
            QStringList Ur_names;
            QStringList Ur_id;
            query->seek(-1);//вернуть указатель в начало
            while (query->next()) {
                QSqlRecord record = query->record();
                Ur_id.append(record.value("id").toString());
                Ur_names.append(record.value("Ur_name").toString());
            }
            AddNewOrder->SetComboBoxClientsId(Ur_names, Ur_id);
            AddNewOrder->exec();
            if (AddNewOrder->ModalResult == 1) {
                QSqlQuery *query = new QSqlQuery();
                query->prepare("INSERT INTO `Orders_` (Client_id, Total_sum, Payment_status) VALUES (:ClientId, :TotalSum, :PaymentStatus)");

                query->bindValue(":ClientId",AddNewOrder->Ur_Id);
                query->bindValue(":TotalSum",AddNewOrder->Order_summ);
                query->bindValue(":PaymentStatus",AddNewOrder->Order_status);
                query->exec();

                Additional *Add = new Additional();
                Add->GetLastQueryError(query);

                this->current_sdb.commit();
                this->filterOrders();//применим фильтрацию
                Additional::ShowMessage("Запись о новом заказе успешно добавлена");
                return;
            }
            else if (AddNewOrder->ModalResult == 2) {
                Additional::ShowMessage("Ошибка добавления заказа\nПопробуйте ещё раз");
                return;
            }
            else if (AddNewOrder->ModalResult == -1) {
                return;
            }
        }
        if(size == 0) {
            Additional::ShowMessage("В базе нет ни одного клиента!\nСначала добавьте клиента\т Для добавления используйте справочник 'Клиенты'");
        }
    }
    catch (...) {
        Additional::ShowMessage("Ошибка!");
        return;
    }
}

/*
 *Удалить выбранный заказ
 *
 */
void MainWindow::DeleteSelectedOrder() {
    int rowidx = ui->TableViewOrders->selectionModel()->currentIndex().row();
    if (rowidx == -1) {
        return;
    }
    int reply = QMessageBox::question(this, "Very big spawn", "Будет удалена выбранная запись",
                                    QMessageBox::Yes|QMessageBox::Cancel);
    if (reply == QMessageBox::Yes) {
        try {

            QString OldOrderId = ui->TableViewOrders->model()->index(rowidx,0).data().toString();
            QSqlQuery *query_delete = new QSqlQuery();
            query_delete->prepare("DELETE FROM `Orders_` WHERE Order_id == :Id");
            query_delete->bindValue(":Id",OldOrderId);
            query_delete->exec();

            Additional *Add = new Additional();
            Add->GetLastQueryError(query_delete);

            this->current_sdb.commit();
            this->filterOrders();//применим фильтрацию
            Additional::ShowMessage("Запись о заказе успешно удалена");
            return;
        }
        catch (...) {
            Additional::ShowMessage("Ошибка удаления записи!");
        }
    }
    else {
        return;
    }

}

/*
 *Редактировать выбранный заказ
 *
 */
void MainWindow::EditSelectedOrder(){
    try {

            int rowidx = ui->TableViewOrders->selectionModel()->currentIndex().row();
            if (rowidx == -1) {
                return;
            }

            QString OldOrderId = ui->TableViewOrders->model()->index(rowidx,0).data().toString();

            QString CurrentClientOrderId = ui->TableViewOrders->model()->index(rowidx,12).data().toString();
            QString OrderStatus = ui->TableViewOrders->model()->index(rowidx,2).data().toString();
            QString OrderSum = ui->TableViewOrders->model()->index(rowidx,1).data().toString();

            if (OrderStatus=="Оплачен") {
                OrderStatus = "1";
            }
            if (OrderStatus == "Не оплачен") {
                OrderStatus = "0";
            }

            DialogAddEditOrder *AddNewOrder = new DialogAddEditOrder();
            AddNewOrder->ConfirmationText = "Будет изменён выбранный заказ?";

            QStringList Ur_names;
            QStringList Ur_id;
            /*
             *Запрос всех клиентов
             */
            QSqlQuery *query = new QSqlQuery();
            query->exec("SELECT Id, Ur_name FROM Clients_");
            Additional *Add = new Additional();
            Add->GetLastQueryError(query);
            while (query->next()) {
                QSqlRecord record = query->record();
                Ur_id.append(record.value("id").toString());
                Ur_names.append(record.value("Ur_name").toString());
            }

            AddNewOrder->SetComboBoxClientsId(Ur_names, Ur_id);
            AddNewOrder->Ur_Id = CurrentClientOrderId;
            AddNewOrder->Order_status = OrderStatus;
            AddNewOrder->Order_summ = OrderSum;
            AddNewOrder->SetOldOrderInformation();

            AddNewOrder->exec();
            if (AddNewOrder->ModalResult == 1) {
                QSqlQuery *query = new QSqlQuery();
                query->prepare("UPDATE `Orders_` Set Client_id = :ClientId, Total_sum = :TotalSum, Payment_status = :PaymentStatus, Edited_datetime = :EditedDatetime WHERE Order_id == :OrderId");

                query->bindValue(":ClientId",AddNewOrder->Ur_Id);
                query->bindValue(":TotalSum",AddNewOrder->Order_summ);
                query->bindValue(":PaymentStatus",AddNewOrder->Order_status);
                query->bindValue(":EditedDatetime",QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss"));
                query->bindValue(":OrderId",OldOrderId);

                query->exec();

                Additional *Add = new Additional();
                Add->GetLastQueryError(query);

                this->current_sdb.commit();
                this->filterOrders();//применим фильтрацию
                Additional::ShowMessage("Запись о заказе успешно изменена");
                return;
            }
            else if (AddNewOrder->ModalResult == 2) {
                Additional::ShowMessage("Ошибка изменения заказа\nПопробуйте ещё раз");
                return;
            }
            else if (AddNewOrder->ModalResult == -1) {
                return;
            }
        }
    catch (...) {
        Additional::ShowMessage("Ошибка!");
        return;
    }
}

void MainWindow::ShowAboutDlg() {
    DialogAbout *AboutDlg = new DialogAbout();
    AboutDlg->exec();
}

/*
 *Загрузить смешную картинку
 */
void MainWindow::LoadFunImage() {
    QImage *fn_img = new QImage();
    fn_img->load("flying_penis.png");

    int width_ = fn_img->width();
    int height_ = fn_img->height();

    QFile outputFile("code_fun_image_png.txt");
    outputFile.open(QIODevice::WriteOnly);
    QTextStream outStream(&outputFile);

    outStream << "QImage *image_fun = new QImage("+QString::number(width_)+","+QString::number(height_)+");\n";
    for (int i=0;i<width_;i++) {
        for (int j = 0;j<height_;j++) {
            int red = qRed(fn_img->pixel(i,j));
            int blue = qBlue(fn_img->pixel(i,j));
            int green = qGreen(fn_img->pixel(i,j));
            int alpha = qAlpha(fn_img->pixel(i,j));
            QString code_line = "image_fun->setPixel("+QString::number(i)+","+QString::number(j)+",qRgba("+QString::number(red)+","+QString::number(green)+","+QString::number(blue)+","+QString::number(alpha)+"));\n";
            outStream << code_line;
        }
    }

    /* Close the file */
    outputFile.close();

    /*
     *
     *    QLabel label;
        label.setPixmap(QPixmap::fromImage(image));
        label.show();
     */

}

MainWindow::~MainWindow()
{
    this->current_sdb.close();
    delete ui;
}
