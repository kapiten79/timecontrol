#ifndef CONTROLLER_PROJECTSETTINGS_H
#define CONTROLLER_PROJECTSETTINGS_H

#include <controller_main.h>


class controller_projectSettings : public controller_main
{
    Q_OBJECT
public:
    explicit controller_projectSettings();

    QObject *model;




signals:

    void s_paramsReady(QString);
public slots:
    void create_newProject();
    void init_window();
};

#endif // CONTROLLER_PROJECTSETTINGS_H
