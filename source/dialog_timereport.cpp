#include "dialog_timereport.h"
#include "ui_dialog_timereport.h"

Dialog_timeReport::Dialog_timeReport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_timeReport)
{
    ui->setupUi(this);
}

Dialog_timeReport::~Dialog_timeReport()
{
    delete ui;
}

/* Инициализация пораметров окна */
void Dialog_timeReport::init_window()
{
    qDebug() << "Функция Dialog_timeReport::init_window() запустилась (модель)";
    this->show();
    ctr->model = this;
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit_2->setDate(QDate::currentDate());


    /** Получаем ширину поля таблицы и высчитываем ширину поля
     * заголовка в процентном соотношении */

    int fullWidth, nameWidth;
    fullWidth = ui->tableWidget->width();
    nameWidth = fullWidth - 144 - 160- 144 - 250 -10;

    /** Ширина столбцов */
    ui->tableWidget->setColumnWidth(0,144);
    ui->tableWidget->setColumnWidth(1,144);
    ui->tableWidget->setColumnWidth(2,250);
    ui->tableWidget->setColumnWidth(3,nameWidth);
    ui->tableWidget->setColumnWidth(4,160);

    ctr->get_projList();

}


/* Обработка выбора флага "По всем датам" */
void Dialog_timeReport::on_checkBox_clicked(bool checked)
{
    qDebug() << "Функция Dialog_timeReport::on_checkBox_clicked(bool checked) запустилась (модель)";
    if (checked == true)
    {
        ui->dateEdit->setEnabled(false);
        ui->dateEdit_2->setEnabled(false);
    }
    else
    {
        ui->dateEdit->setEnabled(true);
        ui->dateEdit_2->setEnabled(true);
    }
    ctr->allDate = checked;
}

/* Обработка флага "По всем проектам" */
void Dialog_timeReport::on_checkBox_2_clicked(bool checked)
{
    qDebug() << "Функция Dialog_timeReport::on_checkBox_2_clicked(bool checked) запустилась (модель)";
    if (checked == true)
    {
        ui->lineEdit->setEnabled(false);
        ui->listWidget->setEnabled(false);
        ui->listWidget_2->setEnabled(false);
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
    }
    else
    {
        ui->lineEdit->setEnabled(true);
        ui->listWidget->setEnabled(true);
        ui->listWidget_2->setEnabled(true);
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
    }
    ctr->allProject = checked;
}

/* Добавление названия проекта в общий список проектов */
void Dialog_timeReport::addProj(QString projName)
{
    qDebug() << "Функция Dialog_timeReport::addProj(QString projName) запустилась (модель)";
    ui->listWidget->addItem(projName);
    ui->listWidget->sortItems(Qt::AscendingOrder);

}

/* Поиск проекта в списке по подстроке */
void Dialog_timeReport::on_lineEdit_textChanged(const QString &arg1)
{
    qDebug() << "Функция Dialog_timeReport::on_lineEdit_textChanged(const QString &arg1) запустилась (модель)";
    ui->listWidget->clear();
    ctr->projSubstring = arg1;
    ctr->findProj();
}

/* Добавление выделенного проекта в список отобранных */
void Dialog_timeReport::on_pushButton_clicked()
{
    qDebug() << "Функция Dialog_timeReport::on_pushButton_clicked() запустилась (модель)";
    QString         selProjSt   = ui->listWidget->currentItem()->text();
    int             selRowIndex = ui->listWidget->currentRow();

    ui->listWidget->takeItem(selRowIndex);
    ui->listWidget_2->addItem(selProjSt);
    ui->listWidget->sortItems(Qt::AscendingOrder);
    ctr->selectedProj << selProjSt;
}

/* Удаление выделенного проекта из списока отобранных */
void Dialog_timeReport::on_pushButton_2_clicked()
{
    qDebug() << "Функция Dialog_timeReport::on_pushButton_clicked() запустилась (модель)";
    QString         selProjSt   = ui->listWidget_2->currentItem()->text();
    int             selRowIndex = ui->listWidget_2->currentRow();

    ui->listWidget_2->takeItem(selRowIndex);
    ui->listWidget->addItem(selProjSt);
    ui->listWidget->sortItems(Qt::AscendingOrder);
}

/* Нажатие кнопки формирования отчёта */
void Dialog_timeReport::on_pushButton_3_clicked()
{
    qDebug() << "Функция Dialog_timeReport::on_pushButton_3_clicked() запустилась (модель)";

    ctr->model = this;
    ctr->dateBegin = ui->dateEdit->date();
    ctr->dateEnd   = ui->dateEdit_2->date();
    ctr->processReport();
}

/* Выводим результат отчёта */
void Dialog_timeReport::showResult()
{
    qDebug() << "Функция Dialog_timeReport::showResult() запустилась (модель)";
    int rowCount = ui->tableWidget->rowCount();
    for (int i=0; i<rowCount; i++)
    {
        ui->tableWidget->removeRow(0);
    }

    qDebug() << "Количество строк после чистки " << ui->tableWidget->rowCount();

    for (int i=0; i<ctr->fList.date.count(); i++)
    {
        int newRowNum = ui->tableWidget->rowCount();

        qDebug() << "Номер очередной строки " << newRowNum;
        ui->tableWidget->insertRow(newRowNum);
        QTableWidgetItem * projName              = new QTableWidgetItem(ctr->fList.projectName[i].toString());
        QTableWidgetItem * projDate              = new QTableWidgetItem(ctr->fList.date[i]       .toString());
        QTableWidgetItem * projTaskCount         = new QTableWidgetItem(ctr->fList.taskCount[i]  .toString());
        QTableWidgetItem * projTimeCount         = new QTableWidgetItem(ctr->fList.timeCount[i]  .toString());
        QTableWidgetItem * projMoneyCount        = new QTableWidgetItem(ctr->fList.moneyCount[i] .toString());

        ui->tableWidget->setItem(newRowNum,0,projName        );
        ui->tableWidget->setItem(newRowNum,1,projDate        );
        ui->tableWidget->setItem(newRowNum,2,projTaskCount   );
        ui->tableWidget->setItem(newRowNum,3,projTimeCount   );
        ui->tableWidget->setItem(newRowNum,4,projMoneyCount  );

        ui->label_6->setText(ctr->globalTimeSt);
        ui->label_8->setText(ctr->globalMoneySt);
    }
}

void Dialog_timeReport::on_pushButton_4_clicked()
{
    this->close();
}

/* Экспорт данных отчёта в excel */
void Dialog_timeReport::on_pushButton_5_clicked()
{
    qDebug() << "Функция Dialog_timeReport::on_pushButton_5_clicked() запустилась (модель)";
    controller_exportExcel *cee = new controller_exportExcel;
    cee->topics << "Наименование проекта"<<"Дата последней задачи"<<"Выполненных задач"<<"Потрачено времени"<<"Потрачено денег";
    cee->widthList << "50" << "25" << "25" << "25" << "25" ;
    cee->total << "" << "" << "" << ui->label_6->text() << ui->label_8->text();
    QVector <QString> column1;
    QVector <QString> column2;
    QVector <QString> column3;
    QVector <QString> column4;
    QVector <QString> column5;
    for (int i=0; i<ui->tableWidget->rowCount(); i++)
    {
        column1.push_back(ui->tableWidget->item(i,0)->text());
        column2.push_back(ui->tableWidget->item(i,1)->text());
        column3.push_back(ui->tableWidget->item(i,2)->text());
        column4.push_back(ui->tableWidget->item(i,3)->text());
        column5.push_back(ui->tableWidget->item(i,4)->text());
    }

    cee->qv.push_back(column1);
    cee->qv.push_back(column2);
    cee->qv.push_back(column3);
    cee->qv.push_back(column4);
    cee->qv.push_back(column5);
    cee->reportName = "По времени ";
    qDebug() << "Данные собраны. Строк " << column1.count() << " столбцов " <<cee->qv.count();
    cee->process_export();
}
