#ifndef DIALOG_TIMEREPORT_H
#define DIALOG_TIMEREPORT_H

#include <QDialog>
#include <QDebug>
#include <controller_timereport.h>
#include <controller_exportexcel.h>
#include <QVector2D>

namespace Ui {
class Dialog_timeReport;
}

class Dialog_timeReport : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_timeReport(QWidget *parent = 0);
    ~Dialog_timeReport();

    controller_timereport *ctr = new controller_timereport;
    void init_window();

public slots:
    void addProj(QString);
    void showResult();

private slots:

    void on_checkBox_clicked(bool checked);

    void on_checkBox_2_clicked(bool checked);

    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::Dialog_timeReport *ui;
};

#endif // DIALOG_TIMEREPORT_H
