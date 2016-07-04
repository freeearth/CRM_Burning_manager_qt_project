#ifndef DIALOGFUNFORM_H
#define DIALOGFUNFORM_H

#include <QDialog>
#include <QImage>

namespace Ui {
class DialogFunForm;
}

class DialogFunForm : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogFunForm(QWidget *parent = 0);
    void SetLabel_PixMap(QImage fun_img);
    ~DialogFunForm();
    
private:
    Ui::DialogFunForm *ui;
};

#endif // DIALOGFUNFORM_H
