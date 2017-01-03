#ifndef DIALOG_PROJECTSETTINGS_H
#define DIALOG_PROJECTSETTINGS_H

#include <QDialog>
#include <QDebug>

#include <dialog_createtask.h>
#include <controller_projectsettings.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>

#include <QVariantList>

namespace Ui {
class Dialog_projectSettings;
}

class Dialog_projectSettings : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_projectSettings(QWidget *parent = 0);
    ~Dialog_projectSettings();

    controller_projectSettings cps;
    QObject *parentObj;
    QString caller;



public slots:
    void init_window(QString p_caller);

private slots:

    void on_buttonBoxAcceptDecline_accepted();

    void on_pushButton_clicked();

private:
    Ui::Dialog_projectSettings *ui;

signals:
    void s_setProjParams();
    void s_setProjParams(QString);

};

#endif // DIALOG_PROJECTSETTINGS_H
