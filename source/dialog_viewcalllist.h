#ifndef DIALOG_VIEWCALLLIST_H
#define DIALOG_VIEWCALLLIST_H

#include <QDialog>
#include <controller_viewcalllist.h>

namespace Ui {
class Dialog_viewCallList;
}

class Dialog_viewCallList : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_viewCallList(QWidget *parent = 0);
    ~Dialog_viewCallList();

    void init_windowParams();

    controller_viewCallList cvcl;
private:
    Ui::Dialog_viewCallList *ui;

public slots:

    void loadCallList();
    void setDayColor(QDate editedDate);
    void setClearCalendar();

private slots:
    void on_tableWidget_cellClicked(int row, int column);
    void on_calendarWidget_clicked(const QDate &date);
};

#endif // DIALOG_VIEWCALLLIST_H
