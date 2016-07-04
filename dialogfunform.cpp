#include "dialogfunform.h"
#include "ui_dialogfunform.h"

DialogFunForm::DialogFunForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFunForm)
{
    setWindowIcon(QIcon("stranger.png"));
    ui->setupUi(this);
    ui->labelFun->setScaledContents(true);
}

void DialogFunForm::SetLabel_PixMap(QImage fun_img) {
    ui->labelFun->setPixmap(QPixmap::fromImage(fun_img));
}

DialogFunForm::~DialogFunForm()
{
    delete ui;
}
