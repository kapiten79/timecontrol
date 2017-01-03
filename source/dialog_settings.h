#ifndef DIALOG_SETTINGS_H
#define DIALOG_SETTINGS_H

#include <QDialog>

namespace Ui {
class Dialog_settings;
}

class Dialog_settings : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_settings(QWidget *parent = 0);
    ~Dialog_settings();

private:
    Ui::Dialog_settings *ui;

signals:
    void priceEntered(QString price);

private slots:
    void on_buttonBox_accepted();
};

#endif // DIALOG_SETTINGS_H
