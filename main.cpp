/*
 *пример подключения, записи и запроса из БД
 *sqlite, расположенной в директории программы
 *
 */
#include <QApplication>
#include <QtPlugin>

#include "mainwindow.h"

//Q_IMPORT_PLUGIN(qsqlite4)
//Q_IMPORT_PLUGIN(qjpeg)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.Process();
    w.show();
    
    return a.exec();
}
