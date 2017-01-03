#ifndef CONTROLLER_VIEWCALLLIST_H
#define CONTROLLER_VIEWCALLLIST_H

#include <controller_main.h>

class controller_viewCallList : public controller_main
{
    Q_OBJECT
public:
    controller_viewCallList();
    QObject *model;

signals:

public slots:
    void init_window();

signals:
    void s_loadCallList();
};

#endif // CONTROLLER_VIEWCALLLIST_H
