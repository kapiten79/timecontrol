#ifndef CONTROLLER_FIND_H
#define CONTROLLER_FIND_H

#include <controller_main.h>


class controller_find : public controller_main
{
    Q_OBJECT
public:
    controller_find();

    QObject *model;
    QStringList projDirList;

    bool    allDate             ;
    bool    allProject          ;
    bool    allCalls            ;
    bool    allTasks            ;
    bool    setSubString        ;

    QDate   dateBegin           ;
    QDate   dateEnd             ;

    QString projectName         ;
    QString subString           ;
    QString findWorkDir;

    QStringList qsl;

    struct findList
    {
        QVariantList date       ;
        QVariantList time       ;
        QVariantList projectName;
        QVariantList topic      ;
        QVariantList description;
        QVariantList type       ;
    };

    findList fList;
signals:
    void s_addProj(QString);

public slots:
    void get_projList();
    void process_search();

};

#endif // CONTROLLER_FIND_H
