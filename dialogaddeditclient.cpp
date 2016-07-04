#include "dialogaddeditclient.h"
#include "ui_dialogaddeditclient.h"

DialogAddEditClient::DialogAddEditClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddEditClient)
{
    setWindowIcon(QIcon("stranger.png"));
    ui->setupUi(this);
    this->Process();

    //устанавливаем правила для ввода данных о номере телефона, ОГРН, Р/С счёте
    QRegExp rxPhone("^[0-9\-\+]{5,40}$");
    QValidator *PhoneValidator = new QRegExpValidator(rxPhone,this);
    ui->lineEditAgentPhone->setValidator(PhoneValidator);

    QRegExp rxOGRN("[0-9]{10,40}");
    QValidator *OGRNValidator = new QRegExpValidator(rxOGRN,this);
    ui->lineEditUrOGRN->setValidator(OGRNValidator);

    QRegExp rxCheckingAccount("[0-9]{10,40}");
    QValidator *CheckingAccountValidator = new QRegExpValidator(rxCheckingAccount,this);
    ui->lineEditUrCheckingAccount->setValidator(CheckingAccountValidator );
}

void DialogAddEditClient::Process() {
    connect(ui->pushButtonConfirmation,SIGNAL(clicked()),SLOT(ConfirmationBtnClick()));
    connect(ui->pushButtonExit,SIGNAL(clicked()),SLOT(Exit()));
}


void DialogAddEditClient::ConfirmationBtnClick() {

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Useless Info", this->ConfirmationText,
                                    QMessageBox::Yes|QMessageBox::Cancel);
    if (reply == QMessageBox::Yes) {
        try {
            QString ErrMsg ="";
            this->Ur_Adress = ui->textEditUrAdress->toPlainText();
            this->Ur_Checking_account = ui->lineEditUrCheckingAccount->text();
            this->Ur_Name = ui->textEditUrName->toPlainText();
            this->OGRN = ui->lineEditUrOGRN->text();
            this->Agent_FIO = ui->textEditAgentFIO->toPlainText();
            this->Agent_Email = ui->textEditAgentEmail->toPlainText();
            this->Agent_Phone = ui->lineEditAgentPhone->text();

            if (this->Ur_Adress.isEmpty()) {
                ErrMsg = ErrMsg + "Адрес юридического лица\n";
            }
            if (this->OGRN.isEmpty()) {
                ErrMsg = ErrMsg + "ОГРН\n";
            }
            if (this->Ur_Name.isEmpty()) {
                ErrMsg = ErrMsg + "Полное наименование юр.лица\n";
            }
            if (this->Ur_Checking_account.isEmpty()) {
                ErrMsg = ErrMsg + "Расчётный счёт\n";
            }
            if (this->Agent_FIO.isEmpty()) {
                ErrMsg = ErrMsg + "ФИО агента\n";
            }
            if (this->Agent_Email.isEmpty()) {
                ErrMsg = ErrMsg + "Email агента\n";
            }
            if (this->Agent_Phone.isEmpty()) {
                ErrMsg = ErrMsg + "Телефон агента\n";
            }

            if (!ErrMsg.trimmed().isEmpty()) {
                Additional::ShowMessage("Не заполнены следующие поля:\n"+ErrMsg+"\Пожалуйста заполните указанные поля и повторите попытку");
                return;
            }
            this->Modalresult = 1;
            this->close();
        }
        catch (...) {
            this->Modalresult = 2;
        }
    }
    else {
        return;
    }
}

bool DialogAddEditClient::SetOldClientInformation() {
    try {
        ui->lineEditAgentPhone->setText(this->Agent_Phone);
        ui->lineEditUrOGRN->setText(this->OGRN);
        ui->lineEditUrCheckingAccount->setText(this->Ur_Checking_account);
        ui->textEditAgentEmail->setText(this->Agent_Email);
        ui->textEditAgentFIO->setText(this->Agent_FIO);
        ui->textEditUrAdress->setText(this->Ur_Adress);
        ui->textEditUrName->setText(this->Ur_Name);
        return true;
    }
    catch (...) {
        Additional::ShowMessage("Ошибка");
        return false;
    }
}

void DialogAddEditClient::Exit() {
    this->close();
    return;
}

DialogAddEditClient::~DialogAddEditClient()
{
    delete ui;
}
