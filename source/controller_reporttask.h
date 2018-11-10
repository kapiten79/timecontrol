#ifndef CONTROLLER_REPORTTASK_H
#define CONTROLLER_REPORTTASK_H

#include <QObject>
#include <controller_main.h>

class controller_reporttask : public controller_main
{
    Q_OBJECT
public:
    explicit controller_reporttask();

    QObject *model;

    QStringList qsl;
    QStringList projDirList;

    QString selectedProj;

    QDate dateBegin;
    QDate dateEnd;

    struct findList
    {
        QVariantList date               ;
        QVariantList taskName           ;
        QVariantList timeCount          ;
        QVariantList moneyCount         ;
        QVariantList taskDeskription    ;
    };

    findList fList;

    QString globalTimeSt;
    QString globalMoneySt;

signals:

public slots:

    void get_projList();
    void processReport();

signals:
    void s_addProj(QString);
    void s_result();
};

#endif // CONTROLLER_REPORTTASK_H
