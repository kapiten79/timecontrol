#ifndef DIALOG_CREATETASK_H
#define DIALOG_CREATETASK_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class Dialog_createTask;
}

class Dialog_createTask : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_createTask(QWidget *parent = 0);
    ~Dialog_createTask();
    QString labelText;
    void init_windowParams();
private slots:
    void on_buttonBox_accepted();
    
private:
    Ui::Dialog_createTask *ui;

signals:
    void nameEntered(QString);
};

#endif // DIALOG_CREATETASK_H
