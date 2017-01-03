#ifndef CONTROLLER_TIMEREPORT_H
#define CONTROLLER_TIMEREPORT_H

#include<controller_main.h>
#include <QTime>

class controller_timereport : public controller_main
{
    Q_OBJECT
public:
    explicit controller_timereport();
    QObject *model;
    QStringList qsl;
    QStringList projDirList;

    QString projSubstring;

    bool    allDate     = false;
    bool    allProject  = false;

    QDate dateBegin;
    QDate dateEnd;

    QStringList selectedProj;

    struct findList
    {
        QVariantList date           ;
        QVariantList projectName    ;
        QVariantList taskCount      ;
        QVariantList timeCount      ;
        QVariantList moneyCount     ;
    };

    findList fList;

    QString globalTimeSt;
    QString globalMoneySt;

signals:

public slots:
    void get_projList   ();
    void findProj       ();
    void processReport  ();
signals:
    void s_addProj(QString);
    void s_result();
};

#endif // CONTROLLER_TIMEREPORT_H
