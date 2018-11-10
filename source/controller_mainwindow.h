#ifndef CONTROLLER_MAINWINDOW_H
#define CONTROLLER_MAINWINDOW_H

#include <controller_main.h>

#include <QTimer>

#include <dialog_createtask.h>
#include <dialog_projectsettings.h>
#include <dialog_createcall.h>
#include <dialog_info.h>
#include <dialog_viewcalllist.h>
#include <dialog_find.h>

#ifdef linux
    #include <cmath>
    #include <ctgmath>
#endif

class controller_mainWindow: public controller_main
{
    Q_OBJECT

public:
    controller_mainWindow() ;

    QObject         *model          ;
    int             currTaskNum     ; // Номер текущей задачи (в рамках дня)
    int             currIndexNum    ; // Номер текущей задачи (в рамках дня)
    QString         dayTime         ; // Количество времени, потраченного за день
    QString         dayPrice        ; // Количество денег, потраченных за день
    QString         allTime         ; // Количество времени, потраченного за весь проект
    QString         allPrice        ; // количество денег, потраченных за весь проект



    QVariantList    projList        ; // Список открытых проектов
    QVariantList    taskNumList     ; // Список номеров задач в рамках дня
    QVariantList    indexNumList    ; // Список индексов задач в общем списке


    Dialog_projectSettings *dps = new Dialog_projectSettings;
    QTimer *timer               = new QTimer                ;
    Dialog_createTask   *dct    = new Dialog_createTask     ;
    Dialog_info         *di     = new Dialog_info           ;

    bool errorFlag = false;


private:


    int rublCount = 0           ;
    int kopCount  = 0           ;

    QVariantList fullText       ;



public slots:
    void init_window            (int flag = 1)      ;
    void open_createTaskDialog  ()                  ;
    void open_removeTaskDialog  ()                  ;
    void startTaskTimer         ()                  ;
    void stopTaskTimer          ()                  ;
    void setDay                 (int flag = 1)                  ;
    void addTask                (QString taskName)  ;
    void saveDay                ()                  ;
    //void setWorkDir             (QString workDir)   ;
    void open_projSettings      (bool)           ;
    void setProjParams          ()                  ;
    void setTodayDateTime       ()                  ;
    void open_createCallDialog  (bool)           ;
    void open_viewCallDialog    (bool)           ;
    void open_dialogFind        (bool tr)           ;
    void create_project         ()                  ;
    void move_task              ()                  ;
    void open_confirmDialog     (QString text)      ;
    void save_project           (bool)           ;
    void open_project           ()                  ;
    void close_project          (int index)         ;

private slots:
    void reloadTaskList (QString saveFlag)  ;
    void setTime        ()                  ;
    void calcDayTime    ()                  ;
    void calcTaskPrice  ()                  ;
    void calcDayPrice   ()                  ;
    void removeCurrTask ()                  ;

signals:
    void s_reloadTaskList   ()      ;
    void s_setTaskTime      ()      ;
    void s_setTaskPrice     ()      ;
    void s_setTaskDayTime   ()      ;
    void s_setTaskDayPrice  ()      ;
    void s_removeCurrTask   ()      ;
    void s_setDayColor      (QDate) ;
    void s_setClearCalendar ()      ;

};

#endif // CONTROLLER_MAINWINDOW_H
