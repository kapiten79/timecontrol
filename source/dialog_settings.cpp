#include "dialog_settings.h"
#include "ui_dialog_settings.h"

Dialog_settings::Dialog_settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_settings)
{
    ui->setupUi(this);

}

Dialog_settings::~Dialog_settings()
{
    delete ui;
}

void Dialog_settings::on_buttonBox_accepted()
{
    emit priceEntered(ui->lineEdit->text());
}
