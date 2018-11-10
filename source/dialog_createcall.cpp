#include "dialog_createcall.h"
#include "ui_dialog_createcall.h"

Dialog_createCall::Dialog_createCall(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_createCall)
{
    ui->setupUi(this);
}

Dialog_createCall::~Dialog_createCall()
{
    delete ui;
}

void Dialog_createCall::init_window()
{
    qDebug() << "Функция init_window запустилась";
    this->setWindowTitle("Новый звонок");
    this->open();
    ccc.model = this;
    ccc.get_projList();
    on_comboBox_activated(0);
}

void Dialog_createCall::addProj(QString proj)
{
    qDebug() << "Функция addProj запустилась";
    ui->comboBox->addItem(proj);
}


void Dialog_createCall::on_checkBox_clicked(bool checked)
{
    qDebug() << "Функция on_checkBox_clicked запустилась";
    if (checked == true)
    {
        ui->comboBox->setEnabled(false);
    }
    else
    {
        ui->comboBox->setEnabled(true);
    }
}

/* Нажатие кнопки "ок" в диалоге */
void Dialog_createCall::on_buttonBox_accepted()
{
    qDebug() << "Функция on_buttonBox_accepted запустилась ";

    ccc.cList.callDate          << ccc.calendarDate.toString("dd_MM_yyyy")                      ;
    ccc.cList.callTime          << ccc.currTime.toString("hh:mm:ss")                            ;
    ccc.cList.callIndex         << ccc.cList.callIndex.count()                                  ;
    ccc.cList.callDescription   << ui->plainTextEdit->toPlainText()                             ;
    ccc.cList.callTopics        << ui->lineEdit->text()                                         ;
    if (ui->checkBox->isChecked())
    {
        ccc.press_ok(true,"");
    }
    else
    {
        ccc.press_ok(false,ui->comboBox->currentText())     ;
    }

}

/* Назначение каталога для сохранения в зависимости от выбранного проекта */
void Dialog_createCall::on_comboBox_activated(int )
{
    qDebug() << "Функция on_comboBox_activated запустилась ";
    //ccc.workDir = "project/"+ccc.projDirList[index].toString();
    ccc.get_oldCalls();
}
