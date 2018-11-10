#ifndef CONTROLLER_CREATECALL_H
#define CONTROLLER_CREATECALL_H

#include <controller_main.h>
#include <dialog_projectsettings.h>

class controller_createCall : public controller_main
{
    Q_OBJECT
public:
    controller_createCall();

    QObject *model;

    QVariantList    projDirList     ;
private:
    Dialog_projectSettings *dps = new Dialog_projectSettings;

signals:

public slots:
    void get_projList       ()                              ;
    void press_ok           (bool newProj, QString projName);
    void saveCall           (QString projName)              ;
    void setTodayDateTime   ()                              ;
    void get_oldCalls       ()                              ;

signals:
    void s_addProj(QString);


};

#endif // CONTROLLER_CREATECALL_H
