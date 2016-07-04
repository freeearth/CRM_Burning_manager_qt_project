#include "additional.h"

Additional::Additional(QObject *parent) :
    QObject(parent)
{
}

/*
  Вывод пользовательского сообщения
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
 *Получить последнюю ошибку при запросе
 *для QSqlQueryModel, QSqlTableModel и QSqlQuery
 *3 перегруженных функции
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
    *Устанавливаем названия столбцов в таблице TableView
    *
    *
    */
   model_query->setHeaderData(0, Qt::Horizontal, "Номер заказа");
   model_query->setHeaderData(1, Qt::Horizontal, "Сумма");
   model_query->setHeaderData(2, Qt::Horizontal, "Статус оплаты");
   model_query->setHeaderData(3, Qt::Horizontal, "Дата и время заказа");
   model_query->setHeaderData(4, Qt::Horizontal, "Дата и время последнего редактирования");
   model_query->setHeaderData(5, Qt::Horizontal, "Полное наименование юр.лица");
   model_query->setHeaderData(6, Qt::Horizontal, "Юридический адрес");
   model_query->setHeaderData(7, Qt::Horizontal, "ОГРН");
   model_query->setHeaderData(8, Qt::Horizontal, "Расчётный счёт");
   model_query->setHeaderData(9, Qt::Horizontal, "ФИО агента");
   model_query->setHeaderData(10, Qt::Horizontal, "Email агента");
   model_query->setHeaderData(11, Qt::Horizontal, "Рабочий телефон");
}

void Additional::SetModelHeaderClients(QSqlQueryModel *model_query) {
    /*
    *Устанавливаем названия столбцов в таблице TableView
    *
    *
    */
   model_query->setHeaderData(0, Qt::Horizontal, "Уникальный номер");
   model_query->setHeaderData(1, Qt::Horizontal, "Полное наименование юр.лица");
   model_query->setHeaderData(2, Qt::Horizontal, "Юридический адрес");
   model_query->setHeaderData(3, Qt::Horizontal, "ОГРН");
   model_query->setHeaderData(4, Qt::Horizontal, "Расчётный счёт");
   model_query->setHeaderData(5, Qt::Horizontal, "ФИО агента");
   model_query->setHeaderData(6, Qt::Horizontal, "Email агента");
   model_query->setHeaderData(7, Qt::Horizontal, "Рабочий телефон");
   model_query->setHeaderData(8, Qt::Horizontal, "Дата регистрации");
   model_query->setHeaderData(9, Qt::Horizontal, "Дата последнего изменения");
}

