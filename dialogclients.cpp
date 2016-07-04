#include "dialogclients.h"
#include "ui_dialogclients.h"

DialogClients::DialogClients(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogClients)
{
    //для правильного отображения кириллицы
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("CP-1251"));
    setWindowIcon(QIcon("stranger.png"));

    ui->setupUi(this);
    QString filter_col_names[10] = {"Уникальный номер","Наименование юр.лица","Адрес юр.лица", "ОГРН", "Расчётный счёт","ФИО агента", "Email агента","Телефон агента","Дата и время регистрации\nYYYY-MM-DD hh:mm:ss", "Дата и время редактирования\nYYYY-MM-DD hh:mm:ss"};

    for (int i = 0; i < sizeof(filter_col_names)/sizeof(QString);i++) {
        ui->comboBoxFilterClient->addItem(filter_col_names[i], this->filter_col_bd_names[i]);
    }

    this->Process();
    this->getInitialClients();
}


void DialogClients::SetCurrentDbConnection(QSqlDatabase sdb) {
    this->current_sdb = sdb;
}

void DialogClients::Process() {
    connect(ui->BtnAddNewClient,SIGNAL(clicked()),SLOT(ShowDialogAddEditClient()));
    connect(ui->BtnEditSelectedClient,SIGNAL(clicked()),SLOT(ShowDialogAddEditClient_Edit()));
    connect(ui->lineEditFilteringClient,SIGNAL(textChanged(QString)),SLOT(filterClients()));
    connect(ui->BtnDeleteSelectedClient,SIGNAL(clicked()),SLOT(DeleteSelectedRowClient()));
}


void DialogClients::ShowDialogAddEditClient() {
    DialogAddEditClient *DlgAddEditClients = new DialogAddEditClient();
    DlgAddEditClients->ConfirmationText = "Будет добавлена запись о клиенте?";
    DlgAddEditClients->exec();
    if (DlgAddEditClients->Modalresult == 1) {
        QSqlQuery *query = new QSqlQuery();
        query->prepare("INSERT INTO `Clients_` (Ur_name, Ur_adress, OGRN, Ur_checking_account,Agent_names, Agent_email, Agent_phone) VALUES (:UrName,:UrAdress, :OGRN, :UrCheckingAccount, :AgentNames, :AgentEmail, :AgentPhone)");
        query->bindValue(":UrName",DlgAddEditClients->Ur_Name);
        query->bindValue(":UrAdress",DlgAddEditClients->Ur_Adress);
        query->bindValue(":OGRN",DlgAddEditClients->OGRN);
        query->bindValue(":UrCheckingAccount",DlgAddEditClients->Ur_Checking_account);
        query->bindValue(":AgentNames",DlgAddEditClients->Agent_FIO);
        query->bindValue(":AgentEmail",DlgAddEditClients->Agent_Email);
        query->bindValue(":AgentPhone",DlgAddEditClients->Agent_Phone);
        query->exec();
        Additional *Add = new Additional();
        Add->GetLastQueryError(query);
        this->current_sdb.commit();
        this->filterClients();//применим фильтрацию
        Additional::ShowMessage("Запись о новом клиенте успешно добавлена");
        return;
    }
    else if (DlgAddEditClients->Modalresult == 2) {
        Additional::ShowMessage("Ошибка добавления клиента\nПопробуйте ещё раз");
        return;
    }
    else if (DlgAddEditClients->Modalresult == -1) {
        return;
    }

}


void DialogClients::ShowDialogAddEditClient_Edit() {
    int rowidx = ui->TableViewClients->selectionModel()->currentIndex().row();
    if (rowidx == -1) {
        return;
    }

    DialogAddEditClient *DlgAddEditClients = new DialogAddEditClient();
    DlgAddEditClients->ConfirmationText = "Будет изменена запись о клиенте?";
    QString CurrentId = ui->TableViewClients->model()->index(rowidx,0).data().toString();

    /*
     *передаём в форму текущие значения
     */
    DlgAddEditClients->Ur_Name = ui->TableViewClients->model()->index(rowidx,1).data().toString();
    DlgAddEditClients->Ur_Adress = ui->TableViewClients->model()->index(rowidx,2).data().toString();
    DlgAddEditClients->OGRN = ui->TableViewClients->model()->index(rowidx,3).data().toString();
    DlgAddEditClients->Ur_Checking_account = ui->TableViewClients->model()->index(rowidx,4).data().toString();
    DlgAddEditClients->Agent_FIO = ui->TableViewClients->model()->index(rowidx,5).data().toString();
    DlgAddEditClients->Agent_Email = ui->TableViewClients->model()->index(rowidx,6).data().toString();
    DlgAddEditClients->Agent_Phone = ui->TableViewClients->model()->index(rowidx,7).data().toString();

    bool setOldInformation = DlgAddEditClients->SetOldClientInformation();//устанавливаем в форме переданные значения

    if (setOldInformation == false) {
        Additional::ShowMessage("Ошибка\nПопробуйте ещё раз");
        return;
    }
    DlgAddEditClients->exec();
    if (DlgAddEditClients->Modalresult == 1) {
        QSqlQuery *query = new QSqlQuery();
        query->prepare("Update `Clients_` Set Ur_name = :UrName, Ur_adress = :UrAdress,"
                       "OGRN = :OGRN, Ur_checking_account = :UrCheckingAccount,"
                       "Agent_names = :AgentNames, Agent_email = :AgentEmail, Agent_phone = :AgentPhone, Date_edited = :DateEdited WHERE Id = :Id ");
        query->bindValue(":UrName",DlgAddEditClients->Ur_Name);
        query->bindValue(":UrAdress",DlgAddEditClients->Ur_Adress);
        query->bindValue(":OGRN",DlgAddEditClients->OGRN);
        query->bindValue(":UrCheckingAccount",DlgAddEditClients->Ur_Checking_account);
        query->bindValue(":AgentNames",DlgAddEditClients->Agent_FIO);
        query->bindValue(":AgentEmail",DlgAddEditClients->Agent_Email);
        query->bindValue(":AgentPhone",DlgAddEditClients->Agent_Phone);
        query->bindValue(":DateEdited",QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss"));
        query->bindValue(":Id",CurrentId);
        query->exec();
        Additional *Add = new Additional();
        Add->GetLastQueryError(query);
        this->current_sdb.commit();
        this->filterClients();//применим фильтрацию
        Additional::ShowMessage("Запись о выбранном клиенте успешно изменена");
    }
    else if (DlgAddEditClients->Modalresult == 2) {
        Additional::ShowMessage("Ошибка изменения записи о клиенте\nПопробуйте ещё раз");
        return;
    }
    else if (DlgAddEditClients->Modalresult == -1) {
        return;
    }
}

/*
 *
 *Получить начальный набор клиентов без фильтрации
 *
 */
void DialogClients::getInitialClients() {
            QSqlQueryModel *model_query = new QSqlQueryModel();
            QString queryString = "SELECT * FROM `Clients_`";
            model_query->setQuery(queryString, this->current_sdb);

            Additional *Add = new Additional();
            Add->GetLastQueryError(model_query);

            Additional::SetModelHeaderClients(model_query);
            ui->TableViewClients->setModel(model_query);
            //ui->TableViewClients->setColumnHidden(0,true);
            ui->TableViewClients->horizontalHeader()->setStretchLastSection(true);
}

/*
 *Фильтрация клиентов
 *
 */
void DialogClients::filterClients() {
    try {
        QString FilterValue =ui->lineEditFilteringClient->text().trimmed();
        QString FilterDbColumn =ui->comboBoxFilterClient->itemData(ui->comboBoxFilterClient->currentIndex()).toString();
        QSqlQueryModel *model_query = new QSqlQueryModel();
        QString queryString = "SELECT * FROM `Clients_` WHERE " + FilterDbColumn + " LIKE '%"+FilterValue+"%'";
        model_query->setQuery(queryString, this->current_sdb);
        Additional *Add = new Additional();
        Add->GetLastQueryError(model_query);

        Additional::SetModelHeaderClients(model_query);
        ui->TableViewClients->setModel(model_query);
    }
    catch(...) {
            Additional::ShowMessage("Ошибка фильтрации!");
    }
}

/*
 *удаление выбранной записи
 *
 */
void DialogClients::DeleteSelectedRowClient() {
    int rowidx = ui->TableViewClients->selectionModel()->currentIndex().row();
    if (rowidx == -1) {
        return;
    }

    QString CurrentId = ui->TableViewClients->model()->index(rowidx,0).data().toString();
    try {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Info", "Запись о выбранном клиенте будет удалена",
                                        QMessageBox::Yes|QMessageBox::Cancel);
        if (reply == QMessageBox::Yes) {
            Additional *Add = new Additional();
            //проверка - есть ли у данного клиента заказы
            QSqlQuery *query = new QSqlQuery();
            query->prepare("SELECT COUNT(*) as COUNT FROM `Orders_` WHERE Client_id == :Id");
            query->bindValue(":Id",CurrentId);
            query->exec();
            Add->GetLastQueryError(query);
            int count;
            while (query->next()) {
                QSqlRecord record = query->record();
                count =  record.value("COUNT").toInt();
            }
            if (count>0) {
                Additional::ShowMessage("Удаление не будет выполнено\nУ данного клиента есть активные заказы("+QString::number(count)+")!");
                return;
            }
            if (count == 0) {
                QSqlQuery *query_delete = new QSqlQuery();
                query_delete->prepare("DELETE FROM `Clients_` WHERE Id == :Id");
                query_delete->bindValue(":Id",CurrentId);
                query_delete->exec();
                Add->GetLastQueryError(query_delete);
                this->current_sdb.commit();
                this->filterClients();//применим фильтрацию
                Additional::ShowMessage("Выбранная запись успешно удалена");
            }
        }
    }
    catch (...) {
        Additional::ShowMessage("Ошибка удаления записи о клиенте!");
    }
}


DialogClients::~DialogClients()
{
    delete ui;
}
