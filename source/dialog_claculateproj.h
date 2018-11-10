#ifndef DIALOG_CLACULATEPROJ_H
#define DIALOG_CLACULATEPROJ_H

#include <QWidget>
#include <QDebug>
#include <QModelIndex>

namespace Ui {
class dialog_claculateProj;
}

class dialog_claculateProj : public QWidget
{
    Q_OBJECT

public:
    explicit dialog_claculateProj(QWidget *parent = 0);
    ~dialog_claculateProj();

    /** Основные параметры проекта */
    QString proj_name           ;
    QString proj_description    ;
    QString proj_hourPrice      ;
    QString proj_enshureProcent ;
    QString proj_testProcent    ;
    QString proj_markUp         ;
    QString proj_costPrice      ;

    /** Структура элемента проекта */
    struct element
    {
        int     currElementIndex    ;
        int     parentElementIndex  ;
        QString     task_currTaskName   ;
        QString     task_fullTime       ;
        QString     task_devTime        ;
        QString     task_testTime       ;
        QString     task_ensureTime     ;
        QString     task_fullPrice      ;
        QString     task_devPrice       ;
        QString     task_testPrice      ;
        QString     task_ensurePrice    ;
        QString     task_description    ;
    };

    /** Массив подзадач проекта */
    QList<element> proj_taskList;

private:
    Ui::dialog_claculateProj *ui;

public slots:

    void initWindow();
private slots:
    void on_pushButtonAddTask_clicked();
    void on_pushButtonAddSubTask_clicked();
    void on_treeWidgetProject_clicked(const QModelIndex &);
    void on_lineEditCurrTask_textEdited(const QString &arg1);
};

#endif // DIALOG_CLACULATEPROJ_H
