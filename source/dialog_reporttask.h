#ifndef DIALOG_REPORTTASK_H
#define DIALOG_REPORTTASK_H

#include <QDialog>
#include <QDebug>
#include <controller_reporttask.h>
#include <controller_exportexcel.h>

namespace Ui {
class Dialog_reportTask;
}

class Dialog_reportTask : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_reportTask(QWidget *parent = 0);
    ~Dialog_reportTask();

    void init_window();
    controller_reporttask crt;

private:
    Ui::Dialog_reportTask *ui;


public slots:
    void addProj(QString projName);
    void showResult();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
};

#endif // DIALOG_REPORTTASK_H
