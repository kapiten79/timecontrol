#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QIcon>
#include <QPalette>
#include <QDebug>
#include <QObject>
#include <QTimer>
#include <QFileDialog>
#include <math.h>

#include <dialog_createtask.h>
#include <dialog_info.h>

#include <dialog_settings.h>
#include <dialog_projectsettings.h>
#include <dialog_viewcalllist.h>
#include <dialog_timereport.h>
#include <dialog_reporttask.h>


#include <controller_mainwindow.h>





namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString key1;
    QString key2;
    QString platform;
    QDir mainDir;

public slots:
    void reloadTaskList     ();
    void init_windowParams  ();
    void open_project       ();
private slots:
    void on_pushButton_clicked()                            ;
    void setTaskTime()                                      ;
    void setTaskPrice()                                     ;
    void setDayTime()                                       ;
    void setDayPrice()                                      ;
    void setClearCalendar()                                 ;
    void setDayColor(QDate editedDate)                      ;
    void on_pushButton_2_clicked()                          ;
    void removeCurrTask()                                   ;
    void on_pushButton_3_clicked()                          ;
    void on_plainTextEdit_textChanged()                     ;
    void on_tableWidget_cellClicked(int row, int column)    ;
    void on_pushButton_4_clicked()                          ;
    void set_nextDay(bool tr)                               ;

    void on_calendarWidget_clicked(const QDate &date);

    void resizeEvent(QResizeEvent* event);

    void on_projectSettings_triggered();

    void on_tabWidget_tabBarClicked(int index);

    void on_tabWidget_tabBarDoubleClicked(int index);

    void on_action_triggered();

    void on_action_2_triggered();

    void on_action_3_triggered();

    void on_calendarWidget_currentPageChanged(int year, int month);

    void on_action_5_triggered();

    void on_action_6_triggered();

    void on_tabWidget_tabCloseRequested(int index);

    void on_checkBoxDone_clicked();

private:
    Ui::MainWindow          *ui         ;
    controller_mainWindow   *cmw = new controller_mainWindow         ;
    int                     currentTab  ;



};

#endif // MAINWINDOW_H
