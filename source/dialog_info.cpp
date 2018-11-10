#include "dialog_info.h"
#include "ui_dialog_info.h"

Dialog_info::Dialog_info(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_info)
{
    ui->setupUi(this);

}

Dialog_info::~Dialog_info()
{
    delete ui;
}

void Dialog_info::setInfo()
{
    qDebug() << "Функция setInfo запустилась " << text.length() << text;
    ui->label->setText(title);
    if (text.length() > 40)
    {
        QString currStr40;
        QString currStr;
        QString fullText;
        int lastInd = 0;
        int currInd = 0;
        for (int i=0; i<text.length(); i++)
        {
            currStr40   = text.mid(i,60)            ;
            if (currStr40.length() == 60)
            {
                currInd     = currStr40.lastIndexOf(" ");
                currStr     = text.mid(i,currInd)       ;
                fullText    +=  currStr+"\n";
                lastInd     += currInd;
                i = lastInd;
            }
            else
            {
                currStr     = text.mid(i,currStr40.length()) ;
                fullText += currStr;
                ui->label_2->setText(fullText);
                break;
            }
        }
    }
    else
    {
        ui->label_2->setText(text);
    }
}

void Dialog_info::on_buttonBox_accepted()
{
    emit s_remove();
    emit s_taskMove();
}
