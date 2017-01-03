#include "dialog_find.h"
#include "ui_dialog_find.h"

Dialog_find::Dialog_find(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_find)
{
    ui->setupUi(this);
}

Dialog_find::~Dialog_find()
{
    delete ui;
}


/* Назначение начальных параметров окна */
void Dialog_find::init_window()
{
    qDebug() << "Функция init_window запущена (модель)";
    this->open();
    cf.model = this;
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit_2->setDate(QDate::currentDate());
    cf.get_projList();

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
}

/* Добавление проекта в список проектов при инициализации */
void Dialog_find::addProj(QString projName)
{
    qDebug() << "Функция addProj запустилась (модель)";
    ui->comboBox->insertItem(0, projName);
}

/* Запуск механизма поиска */
void Dialog_find::on_pushButton_clicked()
{
    qDebug() << "Функция on_pushButton_clicked запустилась (модель)";
    cf.allDate      = ui->checkBox->isChecked()  ;
    cf.allProject   = ui->checkBox_2->isChecked();
    cf.allTasks     = ui->checkBox_4->isChecked();
    cf.allCalls     = ui->checkBox_3->isChecked();

    cf.dateBegin    = ui->dateEdit->date()  ;
    cf.dateEnd      = ui->dateEdit_2->date();

    cf.projectName  = ui->comboBox->currentText();

    cf.process_search();
    ui->tableWidget->setRowCount(0);

    qDebug() << ui->tableWidget->rowCount();

    for (int i=0; i<cf.fList.date.count(); i++)
    {
        /** Вывод списка результатов поиска */
        /** Добавление новой строки для отображения результатов */
        int newRowNum = ui->tableWidget->rowCount();

        ui->tableWidget->insertRow(newRowNum);

        QTableWidgetItem * date         = new QTableWidgetItem(cf.fList.date        [i].toString());
        QTableWidgetItem * time         = new QTableWidgetItem(cf.fList.time        [i].toString());
        QTableWidgetItem * projectName  = new QTableWidgetItem(cf.fList.projectName [i].toString());
        QTableWidgetItem * topic        = new QTableWidgetItem(cf.fList.topic       [i].toString());
        QTableWidgetItem * type         = new QTableWidgetItem(cf.fList.type        [i].toString());

        /** Запрещаем какое либо редактирование элементов таблицы  */
        date        ->setFlags(Qt::NoItemFlags);
        time        ->setFlags(Qt::NoItemFlags);
        projectName ->setFlags(Qt::NoItemFlags);
        type        ->setFlags(Qt::NoItemFlags);

        ui->tableWidget->setItem(newRowNum,0,date           );
        ui->tableWidget->setItem(newRowNum,1,time           );
        ui->tableWidget->setItem(newRowNum,2,projectName    );
        ui->tableWidget->setItem(newRowNum,3,topic          );
        ui->tableWidget->setItem(newRowNum,4,type           );

        for (int i=0; i<ui->tableWidget->rowCount(); i++)
        {
            ui->tableWidget->item(i,0)->setSelected(false);
            ui->tableWidget->item(i,1)->setSelected(false);
            ui->tableWidget->item(i,2)->setSelected(false);
            ui->tableWidget->item(i,3)->setSelected(false);
            ui->tableWidget->item(i,4)->setSelected(false);
        }

        ui->tableWidget->item(0,0)->setSelected(true);
        ui->tableWidget->item(0,1)->setSelected(true);
        ui->tableWidget->item(0,2)->setSelected(true);
        ui->tableWidget->item(0,3)->setSelected(true);
        ui->tableWidget->item(0,4)->setSelected(true);

        ui->plainTextEdit->setPlainText(cf.fList.description[0].toString());
        ui->plainTextEdit->setFocus();
    }

}

/* Обработка реакции интерфейса на выбор флага "Все даты" */
void Dialog_find::on_checkBox_clicked(bool checked)
{
    qDebug() << "Функция on_checkBox_clicked запустилась";
    if (checked)
    {
        ui->dateEdit->setEnabled(false);
        ui->dateEdit_2->setEnabled(false);
    }
    else
    {
        ui->dateEdit->setEnabled(true);
        ui->dateEdit_2->setEnabled(true);
    }
}

/* Обработка реакции интерфейса на выбор флага "Все проекты" */
void Dialog_find::on_checkBox_2_clicked(bool checked)
{
    qDebug() << "Функция on_checkBox_2_clicked запустилась";
    if (checked)
    {
        ui->comboBox->setEnabled(false);
    }
    else
    {
        ui->comboBox->setEnabled(true);
    }
}

/* Вывод информации и выбранном проекте или звонке */
void Dialog_find::on_tableWidget_cellClicked(int row, int column)
{
    qDebug() << "Функция on_tableWidget_cellClicked запустилась";
    ui->plainTextEdit->setPlainText(cf.fList.description[row].toString());
    ui->plainTextEdit->setFocus();
}

/* Обработка реакции инфтерфейса и на выбор флага "Искать по подстроке" */
void Dialog_find::on_checkBox_5_clicked(bool checked)
{
    qDebug() << "Функция on_checkBox_5_clicked запустилась";
    if (checked)
    {
        ui->lineEdit->setEnabled(true);
        cf.setSubString = true;
    }
    else
    {
        ui->lineEdit->setEnabled(false);
        cf.setSubString = false;
    }
}

/* Обработка реакции инфтерфейса и изменение подстроки поиска */
void Dialog_find::on_lineEdit_textChanged(const QString &arg1)
{
    qDebug() << "Функция on_lineEdit_textChanged запустилась";
    cf.subString = arg1;
}
