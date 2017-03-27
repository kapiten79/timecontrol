#include "controller_reporttask.h"

controller_reporttask::controller_reporttask()
{

}

/* Получение списка папок проектов */
void controller_reporttask::get_projList()
{
    qDebug() << "Функция controller_reporttask::get_projList() запутилась (контроллер) " << *workDir;
    qsl;
    QDir qd(*workDir);
    qd.cd("..");

    qsl = qd.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i=0; i<qsl.count(); i++)
    {
        qDebug() << qsl[i];
        projDirList << qsl[i];
        connect(this, SIGNAL(s_addProj(QString)), model, SLOT(addProj(QString)));
        emit s_addProj(qsl[i]);
        disconnect(this, SIGNAL(s_addProj(QString)), model, SLOT(addProj(QString)));
    }
}


/* Обработка нажатия кнопки формирования отчёта */
void controller_reporttask::processReport()
{
    qDebug() << "Функция controller_reporttask::processReport() запутилась (контроллер) " ;
    connect(this, SIGNAL(s_result()), model, SLOT(showResult()));

    fList.date              .clear();
    fList.moneyCount        .clear();
    fList.taskName          .clear();
    fList.timeCount         .clear();
    fList.taskDeskription   .clear();

    int     globalKop         = 0               ;
    int     globalRub         = 0               ;

    int     globalHour        = 0                    ;
    int     globalMinut       = 0                    ;
    int     globalSecs        = 0                    ;

    QString globalRubSt     ;
    QString globalKopSt     ;

    QString globalHourSt    ;
    QString globalMinutSt   ;
    QString globalSecsSt    ;



    //workDir = QDir::currentPath()+"/Project/"+selectedProj;
    QDir qd(*workDir);
    qd.cd("..");
    QString tempWorkDir = *workDir;
    *workDir = qd.path()+"/"+selectedProj;
    readFromFile();
    *workDir = tempWorkDir;
    qDebug() << "Общее количество задач " << j_dataArray.count();

    /** Объясляем переменные для очередного проекта */
    QDate   nextTaskDate                        ;
    QDate   maximumTaskDate = QDate(1,1,1979)   ;
    int     taskHour        = 0                 ;
    int     taskMinut       = 0                 ;
    int     taskSecs        = 0                 ;

    QString taskHourSt                          ;
    QString taskMinutSt                         ;
    QString taskSecsSt                          ;

    QString     taskKopSt                       ;
    QString     taskRubSt                       ;

    int     taskKop         = 0                 ;
    int     taskRub         = 0                 ;

    int     taskCount       = 0                 ;

    int     indexRub, indexKop;

    QString summSt;

    double nacenka;

    /** Перебираем задачи проекта */
    for (int j = 0; j<j_dataArray.count(); j++)
    {
        nextTaskDate = QDate::fromString(j_dataArray[j].toArray()[0].toString(), "dd_MM_yyyy");



        /** Если очередная дата в проекте попадает в заданный диапазон дат */
        if((nextTaskDate >= dateBegin && nextTaskDate <= dateEnd))
        {
            /** Если очередная задача проекта попала в диапазон дат, заносим её в структуру с результатами */
            fList.taskName          << j_dataArray[j].toArray()[2].toString();
            fList.date              << nextTaskDate.toString("dd/MM/yyyy");
            fList.taskDeskription   << j_dataArray[j].toArray()[5].toString();

            /** Суммируем потраченное время */

            taskHour  = QTime::fromString(j_dataArray[j].toArray()[3].toString(),"hh:mm:ss").hour();
            taskMinut = QTime::fromString(j_dataArray[j].toArray()[3].toString(),"hh:mm:ss").minute();
            if (taskMinut > 59)
            {
                taskHour += 1;
                taskMinut -= 60;
            }
            taskSecs  = QTime::fromString(j_dataArray[j].toArray()[3].toString(),"hh:mm:ss").second();
            if (taskSecs > 59)
            {
                taskMinut += 1;
                taskSecs -= 60;
            }

            /** Заносим полученное время выполнения задачи в структуру с результатами */
            /** Переводим время в строку и добавляем нули там где это нужно */

            taskHourSt = QString::number(taskHour);
            if (taskHour < 10)
            {
                taskHourSt = "0"+taskHourSt;
            }
            taskMinutSt = QString::number(taskMinut);
            if (taskMinut < 10)
            {
                taskMinutSt = "0"+taskMinutSt;
            }
            taskSecsSt = QString::number(taskSecs);
            if (taskSecs < 10)
            {
                taskSecsSt = "0"+taskSecsSt;
            }
            qDebug() << "Общее время выполнения задачи по проекту " << taskHourSt << ":" << taskMinutSt << ":" << taskSecsSt;
            fList.timeCount << taskHourSt + ":" + taskMinutSt + ":" + taskSecsSt;


            /** Получаем потраченные деньги из файла */
            summSt      = j_dataArray[j].toArray()[4].toString();
            indexRub    = summSt.indexOf("руб");
            indexKop    = summSt.indexOf("коп");

            /** Суммируем потраченные деньги */
            taskRub     = summSt.mid(0,indexRub).toInt();
            taskKop     = summSt.mid(indexRub+7, 2).toInt();

            if (taskKop > 99)
            {
                taskRub += 1;
                taskKop -= 100;
            }

            /** Переводим деньги за проект в строку и заносим в структуру с результатами */
            taskRubSt = QString::number(taskRub);
            taskKopSt = QString::number(taskKop);
            if (taskKop < 10)
            {
                taskKopSt = "0"+taskKopSt;
            }
            qDebug () << "Количество денег по проекту " << taskRubSt << " рублей " << taskKopSt << " копеек ";
            fList.moneyCount << taskRubSt +" рублей " + taskKopSt + " копеек ";
        }
        /** Суммируем деньги за весь период */
        globalRub +=  taskRub;
        globalKop +=  taskKop;
        if (globalKop > 99)
        {
            globalRub += 1;
            globalKop -= 100;
        }

        /** Суммируем время за весь период*/
        globalHour  += taskHour;
        globalMinut += taskMinut;
        if (globalMinut > 59)
        {
            globalHour  += 1;
            globalMinut -= 60;
        }
        globalSecs  += taskSecs;
        if (globalSecs > 59)
        {
            globalMinut += 1;
            globalSecs  -= 60;
        }
    }




    /** Переводим деньги за весь период в строку */
    globalRubSt = QString::number(globalRub);
    globalKopSt = QString::number(globalKop);
    if (globalKop < 10)
    {
        globalKopSt = "0"+globalKopSt;
    }

    /** Переводим время за весь период в строку */

    globalHourSt = QString::number(globalHour);
    if (globalHour < 10)
    {
        globalHourSt = "0"+globalHourSt;
    }
    globalMinutSt = QString::number(globalMinut);
    if (globalMinut < 10)
    {
        globalMinutSt = "0"+globalMinutSt;
    }
    globalSecsSt = QString::number(globalSecs);
    if (globalSecs < 10)
    {
        globalSecsSt = "0"+globalSecsSt;
    }

    globalMoneySt   = globalRubSt + " рублей " + globalKopSt + " копеек ";
    globalTimeSt    = globalHourSt + ":" + globalMinutSt + ":" + globalSecsSt;
    emit s_result();

    qDebug() << "Всего денег за период " << globalRub << " рублей " << globalKopSt << " копеек ";
    qDebug() << "Всего потрачено времени за период " << globalHourSt << ":" << globalMinut << ":" << globalSecs;
    disconnect(this, SIGNAL(s_result()), model, SLOT(showResult()));
    //selectedProj.clear();
}
