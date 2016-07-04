#ifndef ADDITIONAL_H
#define ADDITIONAL_H

#include <QObject>
#include <QMessageBox>
#include <QtSql>


class Additional : public QObject
{
    Q_OBJECT
public:
    explicit Additional(QObject *parent = 0);
    static void ShowMessage(QString message);
    void GetLastQueryError(QSqlQueryModel *query_model);
    void GetLastQueryError(QSqlTableModel *table_model);
    void GetLastQueryError(QSqlQuery *query);
    static void SetModelHeaderOrders(QSqlQueryModel *model_query);
    static void SetModelHeaderClients(QSqlQueryModel *model_query);
signals:
    
public slots:
    
};

#endif // ADDITIONAL_H
