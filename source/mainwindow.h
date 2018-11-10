#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QIcon>
#include <QPalette>
#include <QDebug>
#include <QObject>
#include <QTimer>
#include <QFileDialog>
#include <QFontDatabase>
#include <QTextCursor>
#include <math.h>

#include <dialog_createtask.h>
#include <dialog_info.h>

#include <dialog_settings.h>
#include <dialog_projectsettings.h>
#include <dialog_viewcalllist.h>
#include <dialog_timereport.h>
#include <dialog_reporttask.h>
#include <dialog_claculateproj.h>

#include <controller_mainwindow.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString key1;
    QString key2;
    QString platform;
    QDir mainDir;

    QTextCursor textCursor;

    QString tempWorkDir; // Нужен на случай, если будет предпринята попытка открыть не верный каталог проекта и потребуется восстановить старый каталог

private:
    QAction *addToolbarAction(QString iconUrl, QString descriptionText, bool enabled = false);

    const int timeColumnWidth   = 144;
    const int priceColumnWidth  = 160;

    const int taskColumn    = 0;
    const int timeColumn    = 1;
    const int priceColumn   = 2;

public slots:
    void reloadTaskList     ();
    void open_project       ();
private slots:
    void on_buttonNewTask_clicked()                            ;
    void setTaskTime()                                      ;
    void setTaskPrice()                                     ;
    void setDayTime()                                       ;
    void setDayPrice()                                      ;
    void setClearCalendar()                                 ;
    void setDayColor(QDate editedDate)                      ;
    void on_buttonStartTaskTimer_clicked()                          ;
    void removeCurrTask()                                   ;
    void on_buttonStopTaskTimer_clicked()                          ;
    void on_plainTextEdit_textChanged()                     ;
    void on_tableForTasks_cellClicked(int row, int)    ;
    void on_buttonDeleteTask_clicked()                          ;
    void set_nextDay(bool)                               ;

    void on_calendarWidget_clicked(const QDate &date);

    void on_projectSettings_triggered();

    void on_tabWidget_tabBarClicked(int index);

    void on_tabWidget_tabBarDoubleClicked(int);

    void on_action_triggered();

    void on_action_2_triggered();

    void on_action_3_triggered();

    void on_calendarWidget_currentPageChanged(int year, int month);

    void on_action_5_triggered();

    void on_action_6_triggered();

    void on_tabWidget_tabCloseRequested(int index);

    void on_checkBoxDone_clicked();

    void on_action_8_triggered();

    void on_action_7_triggered();

    void on_comboBoxFontList_currentIndexChanged(const QString &arg1);

    void on_lineEditFontSize_editingFinished();

    void on_toolButton_clicked();

    void checkCurrentFontParam();

    void on_toolButtonNaklon_clicked();

    void on_toolButtonPodcherk_clicked();

    void on_toolButtonZacherk_clicked();

    void on_plainTextEdit_cursorPositionChanged();

    void on_toolButtonLeft_clicked();

    void on_toolButtonCenter_clicked();

    void on_toolButtonRight_clicked();

private:
    Ui::MainWindow          *ui         ;
    controller_mainWindow   *cmw = new controller_mainWindow         ;
    int                     currentTab  ;



};

#endif // MAINWINDOW_H
