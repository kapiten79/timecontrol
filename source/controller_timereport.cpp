#include "controller_timereport.h"

controller_timereport::controller_timereport()
{

}

/* Получение списка папок проектов */
void controller_timereport::get_projList()
{
    qDebug() << "Функция get_projList запутилась (контроллер) " << *workDir;
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

/* Посик проекта по наименованию и передача списка совпадений в модель */
void controller_timereport::findProj()
{
    qDebug() << "Функция controller_timereport::findProj() запутилась (контроллер)";
    for (int i=0; i<qsl.count(); i++)
    {
        QString currProj = qsl[i];
        if (currProj.indexOf(projSubstring, 0) != -1)
        {
            projDirList << qsl[i];
            connect(this, SIGNAL(s_addProj(QString)), model, SLOT(addProj(QString)));
            emit s_addProj(qsl[i]);
            disconnect(this, SIGNAL(s_addProj(QString)), model, SLOT(addProj(QString)));
        }
    }
}

/* Обработка нажатия кнопки формирования отчёта */
void controller_timereport::processReport()
{
    qDebug() << "Функция controller_timereport::processReport() запутилась (контроллер) " << selectedProj.count();
    connect(this, SIGNAL(s_result()), model, SLOT(showResult()));

    fList.date          .clear();
    fList.moneyCount    .clear();
    fList.projectName   .clear();
    fList.taskCount     .clear();
    fList.timeCount     .clear();


    if(allProject)
    {
        selectedProj = qsl;
    }
    int     globalKop         = 0               ;
    int     globalRub         = 0               ;

    int     globalHour        = 0                    ;
    int     globalMinut       = 0                    ;
    int     globalSecs        = 0                    ;

    QString globalRubSt;
    QString globalKopSt;

    QString globalHourSt;
    QString globalMinutSt;
    QString globalSecsSt;


    /** Перебираем список проектов */
    for (int i=0; i<selectedProj.count(); i++)
    {
        QDir qd(*workDir);
        qd.cd("..");
        QString tempWorkDir = *workDir;
        *workDir = qd.path()+"/"+selectedProj[i];
        readFromFile();
        *workDir = tempWorkDir;
        qDebug() << "Общее количество задач " << j_dataArray.count();

        /** Объясляем переменные для очередного проекта */
        QDate   nextProjDate                        ;
        QDate   maximumProjDate = QDate(1,1,1979)   ;
        int     projHour        = 0                 ;
        int     projMinut       = 0                 ;
        int     projSecs        = 0                 ;

        QString projHourSt                          ;
        QString projMinutSt                         ;
        QString projSecsSt                          ;

        QString projTaskCount                       ;

        QString     projKopSt                       ;
        QString     projRubSt                       ;

        int     projKop         = 0                 ;
        int     projRub         = 0                 ;

        int     taskCount       = 0                 ;

        int     indexRub, indexKop;

        QString summSt;


        /** Заносим название проекта в структуру с результатами */
        fList.projectName << selectedProj[i];

        /** Перебираем задачи проекта */
        for (int j = 0; j<j_dataArray.count(); j++)
        {
            nextProjDate = QDate::fromString(j_dataArray[j].toArray()[0].toString(), "dd_MM_yyyy");

            /** Если очередная дата в проекте попадает в заданный диапазон дат или рассматриваются все даты */
            if((nextProjDate >= dateBegin && nextProjDate <= dateEnd) || allDate)
            {
                /** Если дата задачи больше предыдущей, обновляем её */
                if(nextProjDate > maximumProjDate)
                {
                    maximumProjDate = nextProjDate;
                }

                /** Суммируем количество задач */
                taskCount +=1;

                /** Суммируем потраченное время */

                projHour  += QTime::fromString(j_dataArray[j].toArray()[3].toString(),"hh:mm:ss").hour();
                projMinut += QTime::fromString(j_dataArray[j].toArray()[3].toString(),"hh:mm:ss").minute();
                if (projMinut > 59)
                {
                    projHour += 1;
                    projMinut -= 60;
                }
                projSecs  += QTime::fromString(j_dataArray[j].toArray()[3].toString(),"hh:mm:ss").second();
                if (projSecs > 59)
                {
                    projMinut += 1;
                    projSecs -= 60;
                }


                /** Получаем потраченные деньги из файла */

                summSt      = j_dataArray[j].toArray()[4].toString();
                indexRub    = summSt.indexOf("руб");
                indexKop    = summSt.indexOf("коп");

                /** Суммируем потраченные деньги */
                projRub     += summSt.mid(0,indexRub).toInt();
                projKop     += summSt.mid(indexRub+7, 2).toInt();
                if (projKop > 99)
                {
                    projRub += 1;
                    projKop -= 100;
                }
            }
        }
        /** Переводим деньги за проект в строку */
        projRubSt = QString::number(projRub);
        projKopSt = QString::number(projKop);
        if (projKop < 10)
        {
            projKopSt = "0"+projKopSt;
        }
        qDebug () << "Количество денег по проекту " << projRubSt << " рублей " << projKopSt << " копеек ";
        fList.moneyCount << projRubSt +" рублей " + projKopSt + " копеек ";
        /** Переводим время в строку и добавляем нули там где это нужно */

        projHourSt = QString::number(projHour);
        if (projHour < 10)
        {
            projHourSt = "0"+projHourSt;
        }
        projMinutSt = QString::number(projMinut);
        if (projMinut < 10)
        {
            projMinutSt = "0"+projMinutSt;
        }
        projSecsSt = QString::number(projSecs);
        if (projSecs < 10)
        {
            projSecsSt = "0"+projSecsSt;
        }
        qDebug() << "Общее время выполнения задач по проекту " << projHourSt << ":" << projMinutSt << ":" << projSecsSt;
        fList.timeCount << projHourSt + ":" + projMinutSt + ":" + projSecsSt;

        /** Переводим счётчик выполненных задач за проект в строку */
        projTaskCount = QString::number(taskCount);
        qDebug() << "Счётчик выполненных задач " << projTaskCount;
        fList.taskCount << projTaskCount;

        /** Фиксируем дату последней задачи проекта */
        fList.date << maximumProjDate;

        /** Суммируем деньги за все проекты */
        globalRub +=  projRub;
        globalKop +=  projKop;
        if (globalKop > 99)
        {
            globalRub += 1;
            globalKop -= 100;
        }

        /** Суммируем время за все проекты */
        globalHour  += projHour;
        globalMinut += projMinut;
        if (globalMinut > 59)
        {
            globalHour += 1;
            globalMinut -= 60;
        }
        globalSecs  += projSecs;
        if (globalSecs > 59)
        {
            globalMinut += 1;
            globalSecs -= 60;
        }
    }

    /** Переводим деньги за все проекты в строку */
    globalRubSt = QString::number(globalRub);
    globalKopSt = QString::number(globalKop);
    if (globalKop < 10)
    {
        globalKopSt = "0"+globalKopSt;
    }

    /** Переводим время за все проекты в строку */

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

    qDebug() << "Всего денег за все проекты " << globalRub << " рублей " << globalKopSt << " копеек ";
    qDebug() << "Всего потрачено времени " << globalHourSt << ":" << globalMinut << ":" << globalSecs;
    disconnect(this, SIGNAL(s_result()), model, SLOT(showResult()));
    selectedProj.clear();
}
