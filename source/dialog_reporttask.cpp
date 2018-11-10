#include "dialog_reporttask.h"
#include "ui_dialog_reporttask.h"

Dialog_reportTask::Dialog_reportTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_reportTask)
{
    ui->setupUi(this);
}

Dialog_reportTask::~Dialog_reportTask()
{
    delete ui;
}

/* Инициализация параметров окна */
void Dialog_reportTask::init_window()
{
    qDebug() << "Функциия Dialog_reportTask::init_window() запустилась (модель)";
    /** Получаем ширину поля таблицы и высчитываем ширину поля
     * заголовка в процентном соотношении */

    this->show();

    crt.model = this;

    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit_2->setDate(QDate::currentDate());

    int fullWidth, nameWidth;
    fullWidth = ui->tableWidget->width();
    nameWidth = fullWidth - 240 - 130-18;

    /** Ширина столбцов */
    ui->tableWidget->setColumnWidth(0,120);
    ui->tableWidget->setColumnWidth(1,nameWidth);
    ui->tableWidget->setColumnWidth(2,120);
    ui->tableWidget->setColumnWidth(3,130);


    crt.get_projList();
}

/* Добавление названия проекта в общий список проектов */
void Dialog_reportTask::addProj(QString projName)
{
    qDebug() << "Функция Dialog_reportTask::addProj(QString projName) запустилась (модель)";
    ui->comboBox->addItem(projName);
}

/* Нажатие кнопки "Сформировать" */
void Dialog_reportTask::on_pushButton_clicked()
{
    qDebug() << "Функция Dialog_reportTask::on_pushButton_clicked() запустилась (модель)";
    crt.selectedProj    = ui->comboBox->currentText();
    crt.dateBegin       = ui->dateEdit->date();
    crt.dateEnd         = ui->dateEdit_2->date();
    crt.processReport();
}

/* Вывод результатов */
void Dialog_reportTask::showResult()
{
    qDebug() << "Функция Dialog_reportTask::on_pushButton_clicked() запустилась (модель)";
    int rowCount = ui->tableWidget->rowCount();
    for (int i=0; i<rowCount; i++)
    {
        ui->tableWidget->removeRow(0);
    }

    qDebug() << "Количество строк после чистки " << ui->tableWidget->rowCount();

    for (int i=0; i<crt.fList.date.count(); i++)
    {
        int newRowNum = ui->tableWidget->rowCount();

        ui->tableWidget->insertRow(newRowNum);
        QTableWidgetItem * taskName              = new QTableWidgetItem(crt.fList.taskName[i]   .toString());
        QTableWidgetItem * taskDate              = new QTableWidgetItem(crt.fList.date[i]       .toString());
        QTableWidgetItem * taskTimeCount         = new QTableWidgetItem(crt.fList.timeCount[i]  .toString());
        QTableWidgetItem * taskMoneyCount        = new QTableWidgetItem(crt.fList.moneyCount[i] .toString());

        ui->tableWidget->setItem(newRowNum,0,taskName        );
        ui->tableWidget->setItem(newRowNum,1,taskDate        );
        ui->tableWidget->setItem(newRowNum,2,taskTimeCount   );
        ui->tableWidget->setItem(newRowNum,3,taskMoneyCount  );

        ui->label_8->setText(crt.globalTimeSt);
        ui->label_9->setText(crt.globalMoneySt);
    }
}

/* Нажатие кнопки "экспорт" */
void Dialog_reportTask::on_pushButton_3_clicked()
{
    qDebug() << "Функция Dialog_reportTask::on_pushButton_3_clicked() запустилась (модель)";
    controller_exportExcel *cee = new controller_exportExcel;
    cee->topics << "Задача"<<"Дата"<<"Потрачено времени"<<"Потрачено денег"<<"Пометки";
    cee->widthList << "25" << "50" << "25" << "25" << "100" ;
    cee->total << "" << "" << "" << ui->label_8->text() << ui->label_9->text() << "";
    QVector <QString> column1;
    QVector <QString> column2;
    QVector <QString> column3;
    QVector <QString> column4;
    QVector <QString> column5;
    for (int i=0; i<ui->tableWidget->rowCount(); i++)
    {
        column1.push_back(crt.fList.taskName[i]         .toString());
        column2.push_back(crt.fList.date[i]             .toString());
        column3.push_back(crt.fList.timeCount[i]        .toString());
        column4.push_back(crt.fList.moneyCount[i]       .toString());
        column5.push_back(crt.fList.taskDeskription[i]  .toString());
    }

    cee->qv.push_back(column1);
    cee->qv.push_back(column2);
    cee->qv.push_back(column3);
    cee->qv.push_back(column4);
    cee->qv.push_back(column5);
    cee->reportName = "По задачам за период c " + crt.dateBegin.toString("dd/MM/yyyy")+ " по " + crt.dateEnd.toString("dd/MM/yyyy") + " по проекту '"+crt.selectedProj+"'";
    qDebug() << "Данные собраны. Строк " << column1.count() << " столбцов " <<cee->qv.count();
    cee->process_export();
}

/* Нажатие кнопки закрытия окна отчёта */
void Dialog_reportTask::on_pushButton_2_clicked()
{
    qDebug() << "Функция Dialog_reportTask::on_pushButton_2_clicked() запустилась (модель)";
    this->close();
}
