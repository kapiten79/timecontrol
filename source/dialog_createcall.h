#ifndef DIALOG_CREATECALL_H
#define DIALOG_CREATECALL_H

#include <QDialog>
#include <controller_createcall.h>

namespace Ui {
class Dialog_createCall;
}

class Dialog_createCall : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_createCall(QWidget *parent = 0);
    ~Dialog_createCall();
    controller_createCall ccc;
public slots:
    void init_window()              ;
    void addProj    (QString proj)  ;

private slots:

    void on_checkBox_clicked    (bool checked)  ;
    void on_buttonBox_accepted  ()              ;

    void on_comboBox_activated(int index);

private:
    Ui::Dialog_createCall *ui;
};

#endif // DIALOG_CREATECALL_H
