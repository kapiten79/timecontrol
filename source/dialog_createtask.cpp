#include "dialog_createtask.h"
#include "ui_dialog_createtask.h"

Dialog_createTask::Dialog_createTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_createTask)
{
    ui->setupUi(this);
    ui->lineEdit->setFocus();

}

Dialog_createTask::~Dialog_createTask()
{
    delete ui;
}

/* Инициализируем параметры окна */
void Dialog_createTask::init_windowParams()
{
    qDebug() << "Функция Dialog_createTask::init_windowParams() запустилась";

    this->open();
    ui->label->setText(labelText);
    qDebug() << labelText;

}

/* Нажатие кнопки OK в диалоге */
void Dialog_createTask::on_buttonBox_accepted()
{
    emit nameEntered(ui->lineEdit->text());
    ui->lineEdit->clear();
}
