#include "dialog_viewcalllist.h"
#include "ui_dialog_viewcalllist.h"

Dialog_viewCallList::Dialog_viewCallList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_viewCallList)
{
    ui->setupUi(this);
}

Dialog_viewCallList::~Dialog_viewCallList()
{
    delete ui;
}

void Dialog_viewCallList::init_windowParams()
{
    qDebug() << "Функция init_windowParams запустилась (модель)";

    this->open();
    this->setWindowTitle("История звонков");

    /** Получаем ширину поля таблицы и высчитываем ширину поля
     * заголовка в процентном соотношении */

    int fullWidth;
    fullWidth = ui->tableWidget->width();

    /** Ширина столбцов */
    ui->tableWidget->setColumnWidth(0, 150);
    ui->tableWidget->setColumnWidth(1, fullWidth-170);
    cvcl.model = this;
    cvcl.init_window();
}

/* Загрузка списка звонков за день */
void Dialog_viewCallList::loadCallList()
{
   qDebug() << "Функция loadCallList запустилась (модель)";

   int rowCount = ui->tableWidget->rowCount();

   for (int i=0; i<rowCount; i++)
   {
       ui->tableWidget->removeRow(0);
   }

   QDate nextDate;
   for (int i=0; i<cvcl.cList.callTopics.count(); i++)
   {
       qDebug() << "Заголовок очередного звонка " << cvcl.cList.callTopics[i].toString();
       /** Дата очередного звонка в формате QDate */
       nextDate = QDate::fromString(cvcl.cList.callDate[i].toString(), "dd_MM_yyyy");
       /** Если звонок был принят в выбранном в календаре месяце, отмечаем его дату на календаре */

       if (nextDate.month() == ui->calendarWidget->selectedDate().month() && nextDate.year() == ui->calendarWidget->selectedDate().year())
       {
           setDayColor(nextDate);
       }

       /** Если звонок был выполнен в выбранную в календаре дату, выводим его заголовок */
       int nextRowNum;
       if (nextDate == ui->calendarWidget->selectedDate())
       {

           nextRowNum = ui->tableWidget->rowCount();

           qDebug() << "*******";
           qDebug() << "Очередная строка " << nextRowNum;
           qDebug() << "Очередной заголовок " << cvcl.cList.callTopics[i].toString();
           qDebug() << "Очереднеое время " << cvcl.cList.callTime[i].toString();

           ui->tableWidget->insertRow(nextRowNum);
           QTableWidgetItem * callTime = new QTableWidgetItem(cvcl.cList.callTime[i].toString() );
           QTableWidgetItem * callName = new QTableWidgetItem(cvcl.cList.callTopics[i].toString() );
           ui->tableWidget->setItem(nextRowNum,0,callTime        );
           ui->tableWidget->setItem(nextRowNum,1,callName        );
       }
   }
}

/* Выбор звонка из списка и отображение его текста */
void Dialog_viewCallList::on_tableWidget_cellClicked(int row, int column)
{
    qDebug() << "Функция loadCallList запустилась (модель)";

    QString c_date = ui->calendarWidget->selectedDate().toString("dd_MM_yyyy");
    QString c_time = ui->tableWidget->item(row,0)->text();
    int index;
    for (int i=0; i<cvcl.cList.callDate.count(); i++)
    {
        qDebug() << c_date << " == " << cvcl.cList.callDate[i].toString() << " && " << c_time << " == " << cvcl.cList.callTime[i].toString();
        if (c_date == cvcl.cList.callDate[i].toString() && c_time == cvcl.cList.callTime[i].toString())
        {
            index = i;
            break;
        }
    }
    ui->plainTextEdit->setPlainText(cvcl.cList.callDescription[index].toString());
}

/* Извнение списка звонков при выборе другой даты в календаре */
void Dialog_viewCallList::on_calendarWidget_clicked(const QDate &date)
{
    qDebug() << "Функция Dialog_viewCallList::on_calendarWidget_clicked запустилась (модель)";
    loadCallList();
}

void Dialog_viewCallList::setDayColor(QDate editedDate)
{
    qDebug() << "Функция setDayColor() запустилась (модель) " << editedDate;
    QFont           charFont    ;
    charFont.setBold(true);
    charFont.setPixelSize(12);

    QTextCharFormat charFormat  ;
    charFormat.setFont(charFont);

    ui->calendarWidget->setDateTextFormat(editedDate, charFormat);
}

void Dialog_viewCallList::setClearCalendar()
{
    qDebug() << "Функция setClearCalendar запустилась (модель)";
    QDate editedDate;
    QTextCharFormat charFormat;
    ui->calendarWidget->setDateTextFormat(editedDate, charFormat);
}
