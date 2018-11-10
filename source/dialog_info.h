#ifndef DIALOG_INFO_H
#define DIALOG_INFO_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class Dialog_info;
}

class Dialog_info : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_info(QWidget *parent = 0);
    ~Dialog_info();

    QString title;
    QString text;

    void setInfo();
private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialog_info *ui;

signals:
    void s_remove();
    void s_taskMove();
};

#endif // DIALOG_INFO_H
