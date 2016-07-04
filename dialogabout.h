#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

#include <QDialog>
#include "thirdparty/additional.h"
#include <QImage>
#include "qevent.h"
#include "dialogfunform.h"

namespace Ui {
class DialogAbout;
}

class DialogAbout : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogAbout(QWidget *parent = 0);
    ~DialogAbout();
    bool eventFilter(QObject *obj, QEvent *event);

public slots:

    
private:
    Ui::DialogAbout *ui;
};

#endif // DIALOGABOUT_H
