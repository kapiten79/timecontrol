#ifndef DIALOG_FIND_H
#define DIALOG_FIND_H

#include <QDialog>
#include <controller_find.h>

namespace Ui {
class Dialog_find;
}

class Dialog_find : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_find(QWidget *parent = 0);
    ~Dialog_find();

    controller_find cf;
    void init_window();

private:
    Ui::Dialog_find *ui;

public slots:
    void addProj(QString projName);
private slots:
    void on_pushButton_clicked();
    void on_checkBox_clicked(bool checked);
    void on_checkBox_2_clicked(bool checked);
    void on_tableWidget_cellClicked(int row, int column);
    void on_checkBox_5_clicked(bool checked);
    void on_lineEdit_textChanged(const QString &arg1);
};

#endif // DIALOG_FIND_H
