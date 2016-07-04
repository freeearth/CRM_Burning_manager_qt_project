#include "additional.h"

Additional::Additional(QObject *parent) :
    QObject(parent)
{
}

/*
  ����� ����������������� ���������
*/
void Additional::ShowMessage(QString message) {
    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon("stranger.png"));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(message);
    msgBox.setWindowTitle("Sinking manager");
    msgBox.exec();
}

/*
 *�������� ��������� ������ ��� �������
 *��� QSqlQueryModel, QSqlTableModel � QSqlQuery
 *3 ������������� �������
 */
void Additional::GetLastQueryError(QSqlQueryModel *query_model) {
    if (!query_model->lastError().text().trimmed().isEmpty()) {
        Additional::ShowMessage("SqLite error:" + query_model->lastError().text()+"\n");
        Additional::ShowMessage("SqLite error code:"+ QString::number(query_model->lastError().number() )+"\n");
        return;
    }
}

void Additional::GetLastQueryError(QSqlTableModel *table_model) {
    if (!table_model->lastError().text().trimmed().isEmpty()) {
        Additional::ShowMessage("SqLite error:" + table_model->lastError().text()+"\n");
        Additional::ShowMessage("SqLite error code:"+ QString::number(table_model->lastError().number() )+"\n");
        return;
    }
}

void Additional::GetLastQueryError(QSqlQuery *query) {
    if (!query->lastError().text().trimmed().isEmpty()) {
        Additional::ShowMessage("SqLite error:" + query->lastError().text()+"\n");
        Additional::ShowMessage("SqLite error code:"+ QString::number(query->lastError().number() )+"\n");
        return;
    }
}

void Additional::SetModelHeaderOrders(QSqlQueryModel *model_query) {
    /*
    *������������� �������� �������� � ������� TableView
    *
    *
    */
   model_query->setHeaderData(0, Qt::Horizontal, "����� ������");
   model_query->setHeaderData(1, Qt::Horizontal, "�����");
   model_query->setHeaderData(2, Qt::Horizontal, "������ ������");
   model_query->setHeaderData(3, Qt::Horizontal, "���� � ����� ������");
   model_query->setHeaderData(4, Qt::Horizontal, "���� � ����� ���������� ��������������");
   model_query->setHeaderData(5, Qt::Horizontal, "������ ������������ ��.����");
   model_query->setHeaderData(6, Qt::Horizontal, "����������� �����");
   model_query->setHeaderData(7, Qt::Horizontal, "����");
   model_query->setHeaderData(8, Qt::Horizontal, "��������� ����");
   model_query->setHeaderData(9, Qt::Horizontal, "��� ������");
   model_query->setHeaderData(10, Qt::Horizontal, "Email ������");
   model_query->setHeaderData(11, Qt::Horizontal, "������� �������");
}

void Additional::SetModelHeaderClients(QSqlQueryModel *model_query) {
    /*
    *������������� �������� �������� � ������� TableView
    *
    *
    */
   model_query->setHeaderData(0, Qt::Horizontal, "���������� �����");
   model_query->setHeaderData(1, Qt::Horizontal, "������ ������������ ��.����");
   model_query->setHeaderData(2, Qt::Horizontal, "����������� �����");
   model_query->setHeaderData(3, Qt::Horizontal, "����");
   model_query->setHeaderData(4, Qt::Horizontal, "��������� ����");
   model_query->setHeaderData(5, Qt::Horizontal, "��� ������");
   model_query->setHeaderData(6, Qt::Horizontal, "Email ������");
   model_query->setHeaderData(7, Qt::Horizontal, "������� �������");
   model_query->setHeaderData(8, Qt::Horizontal, "���� �����������");
   model_query->setHeaderData(9, Qt::Horizontal, "���� ���������� ���������");
}

