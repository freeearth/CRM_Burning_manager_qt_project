#include "dialogaddeditorder.h"
#include "ui_dialogaddeditorder.h"

DialogAddEditOrder::DialogAddEditOrder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddEditOrder)
{
    //для правильного отображения кириллицы
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("CP-1251"));
    setWindowIcon(QIcon("stranger.png"));
    ui->setupUi(this);
    this->Process();
    ui->comboBoxOrderStatus->addItem("Не оплачен","0");
    ui->comboBoxOrderStatus->addItem("Оплачен","1");
    QRegExp rxSumm("[0-9]{1,299}");
    QValidator *SummValidator = new QRegExpValidator(rxSumm,this);
    ui->lineEditOrderSumm->setValidator(SummValidator);
}

bool DialogAddEditOrder::SetComboBoxClientsId(QStringList Ur_names, QStringList Id) {
    try {
        for (int i=0;i<Ur_names.count();i++) {
            ui->comboBoxUrNameId->addItem(Ur_names.at(i)+"("+Id.at(i)+")",Id.at(i));
        }
        return true;
    }
    catch (...) {
        Additional::ShowMessage("Ошибка!");
        return false;
    }
}

void DialogAddEditOrder::Process() {
    connect(ui->pushButtonConfirmation,SIGNAL(clicked()),SLOT(ConfirmationBtnClick()));
    connect(ui->pushButtonExit,SIGNAL(clicked()),SLOT(Exit()));
}


void DialogAddEditOrder::ConfirmationBtnClick() {

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Useless Info", this->ConfirmationText,
                                    QMessageBox::Yes|QMessageBox::Cancel);
    if (reply == QMessageBox::Yes) {
        try {
            QString ErrMsg ="";

            this->Ur_Id = ui->comboBoxUrNameId->itemData(ui->comboBoxUrNameId->currentIndex()).toString();
            this->Ur_Name = ui->comboBoxUrNameId->itemText(ui->comboBoxUrNameId->currentIndex());
            this->Order_summ = ui->lineEditOrderSumm->text();
            this->Order_status = ui->comboBoxOrderStatus->itemData(ui->comboBoxOrderStatus->currentIndex()).toString();


            if (this->Order_summ.isEmpty()) {
                ErrMsg = ErrMsg + "Сумма заказа\n";
            }

            if (!ErrMsg.trimmed().isEmpty()) {
                Additional::ShowMessage("Не заполнены следующие поля:\n"+ErrMsg+"\Пожалуйста заполните указанные поля и повторите попытку");
                return;
            }
            this->ModalResult = 1;
            this->close();
        }
        catch (...) {
            this->ModalResult = 2;
        }
    }
    else {
        return;
    }
}


bool DialogAddEditOrder::SetOldOrderInformation() {
    try {
        ui->lineEditOrderSumm->setText(this->Order_summ);

        int index_ur_id = ui->comboBoxUrNameId->findData(this->Ur_Id);
        if (index_ur_id != -1 ) { // -1 for not found
           ui->comboBoxUrNameId->setCurrentIndex(index_ur_id);
        }

        int index_ur_status = ui->comboBoxOrderStatus->findData(this->Order_status);
        if (index_ur_status != -1 ) { // -1 for not found
           ui->comboBoxOrderStatus->setCurrentIndex(index_ur_status);
        }
        return true;
    }
    catch (...) {
        Additional::ShowMessage("Ошибка");
        return false;
    }
}

void DialogAddEditOrder::Exit() {
    this->close();
    return;
}


DialogAddEditOrder::~DialogAddEditOrder()
{
    delete ui;
}
