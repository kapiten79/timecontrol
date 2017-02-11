#include "controller_mainwindow.h"

controller_mainWindow::controller_mainWindow()
{
    setTodayDateTime();
    workDir = new QString(QDir::home().path()); // Ссылка на строку с адресом домашнего каталога пользователя. Используется для хранения каталога текущего проекта.
}

/* Задаётся текущая дата и время */
void controller_mainWindow::setTodayDateTime()
{
    /** Получаем все необходимые значения текущей даты и времени */
    todayDate       = QDate::currentDate()  ;
    calendarDate    = QDate::currentDate()  ;
    currTime        = QTime::currentTime()  ;
}

/* Инициализация параметров окна */
void controller_mainWindow::init_window(int flag)
{
    qDebug() << "Функция controller_mainWindow::init_window запустилась (контроллер) " << flag << " Рабочий каталог " << *workDir << " Текущая дата " << calendarDate.toString("dd.MM.yyyy");

    if (flag == 1)
    {
        readFromFile    ()      ;
        currTaskNum = 0;
        taskNumList .clear();
        indexNumList.clear();

        /** Чистим список задач за весь проект перед обновлением */
        tList.taskDate      .clear();
        tList.taskIndex     .clear();
        tList.taskName      .clear();
        tList.taskTime      .clear();
        tList.taskPrice     .clear();
        tList.taskFullText  .clear();
        tList.taskHour      .clear();
        tList.taskMinute    .clear();
        tList.taskSecond    .clear();
        tList.taskRuble     .clear();
        tList.taskKop       .clear();
        tList.completeFlag  .clear();

        /** Добавляем в структуру taskList задачи за весь проект */
        qDebug() << "Размер массива " << j_dataArray.count();
        for (int i=0; i<j_dataArray.count(); i++)
        {
            tList.taskDate        << j_dataArray[i].toArray()[0].toString();
            tList.taskIndex       << j_dataArray[i].toArray()[1].toString();
            tList.taskName        << j_dataArray[i].toArray()[2].toString();
            tList.taskTime        << j_dataArray[i].toArray()[3].toString();
            tList.taskPrice       << j_dataArray[i].toArray()[4].toString();
            tList.taskFullText    << j_dataArray[i].toArray()[5].toString();
            if (j_dataArray[i].toArray().count() == 7){tList.completeFlag    << j_dataArray[i].toArray()[6].toString();}else{tList.completeFlag    << "0";}

            if (j_dataArray[i].toArray()[3].toString() == "00:00:00")
            {
                tList.taskHour       << 0;
                tList.taskMinute     << 0;
                tList.taskSecond     << 0;

                tList.taskRuble      << 0;
                tList.taskKop        << 0;
            }
            else
            {
                tList.taskSecond << j_dataArray[i].toArray()[3].toString().mid(6,2).toInt();
                tList.taskMinute << j_dataArray[i].toArray()[3].toString().mid(3,2).toInt();
                tList.taskHour   << j_dataArray[i].toArray()[3].toString().mid(0,2).toInt();

                tList.taskRuble      << 0;
                tList.taskKop        << 0;
            }


            /** Обработка данных о задачах за текущий день */
            if (calendarDate.toString("dd_MM_yyyy") == j_dataArray[i].toArray()[0].toString())
            {
                taskNumList     << currTaskNum;
                indexNumList    << j_dataArray[i].toArray()[1].toString();
                currIndexNum = i;
                qDebug() << "Количество элементов массива " << tList.taskName.count();
                qDebug() << "Индекс " << currIndexNum;
                qDebug() << "Для очередной загружаемой задачи " << tList.taskName[currIndexNum] << " Номер индекса " << currIndexNum << " Номер задачи на экране " << currTaskNum;
                currTaskNum += 1;
            }
        }
        /** Для того, чтобы currTaskNum (номер выбранной задачи) нумеровался с нуля, вычитаем из него 1 */
        currTaskNum -= 1;

        reloadTaskList("1");

        /** Читаем файл проекта и наполняем структуру реквизитов проекта данными */
        readProjFromFile();

        if (j_dataArray.count() > 0)
        {
            pList->projNameSt        = j_dataArray[0].toArray()[0].toString();
            pList->projPriceSt       = j_dataArray[0].toArray()[1].toString();
            pList->projDescriptionSt = j_dataArray[0].toArray()[2].toString();
            pList->projContactNameSt = j_dataArray[0].toArray()[3].toString();
            pList->projPhoneSt       = j_dataArray[0].toArray()[4].toString();
            pList->projEmailSt       = j_dataArray[0].toArray()[5].toString();

            curr_hourPrice = pList->projPriceSt;
        }
        else
        {
            /** Если файла проекта пока нет, передаём в структуру стоимость часа по умолчанию */
            pList->projPriceSt = curr_hourPrice;
            qDebug() << "ВЫЗОВ ОКНА ПАРАМЕТРОВ ПРОЕКТА ДЛЯ НАЧАЛЬНОГО ЗАПОЛНЕНИЯ ";
            open_projSettings(true);
        }

        /** Читаем список звонков и заполняем структуру cList */
        readCallFromFile();

        for (int i=j_dataArray.count()-1; i>=0; i--)
        {
            cList.callDate        << j_dataArray[i].toArray()[0].toString();
            cList.callTime        << j_dataArray[i].toArray()[1].toString();
            cList.callIndex       << j_dataArray[i].toArray()[2].toString();
            cList.callProject     << j_dataArray[i].toArray()[3].toString();
            cList.callTopics      << j_dataArray[i].toArray()[4].toString();
            cList.callDescription << j_dataArray[i].toArray()[5].toString();
        }
    }
    else
    {
        /** Обрабатываем календарь */

        QString currTaskDate, trueDate;
        QDate   markDate;

        qDebug() << "Количество дат в tList " << tList.taskDate.count();

        if (tList.taskDate.count() == 0)
        {
            connect (this, SIGNAL(s_setClearCalendar()), model, SLOT(setClearCalendar()));
            emit s_setClearCalendar ();
            disconnect (this, SIGNAL(s_setClearCalendar()), model, SLOT(setClearCalendar()));
        }
        else
        {
            for(int i=0; i<tList.taskDate.count(); i++)
            {
                currTaskDate = tList.taskDate[i].toString();

                qDebug() << currTaskDate.mid(3,7) << " == " << calendarDate.toString("MM_yyyy");
                if (currTaskDate.mid(3,7) == calendarDate.toString("MM_yyyy"))
                {
                    qDebug() << "В условии";
                    trueDate = currTaskDate.mid(0,2);
                    markDate = QDate::fromString(currTaskDate, "dd_MM_yyyy");
                    connect (this, SIGNAL(s_setDayColor(QDate)), model, SLOT(setDayColor(QDate)));
                    emit s_setDayColor(markDate);
                    disconnect (this, SIGNAL(s_setDayColor(QDate)), model, SLOT(setDayColor(QDate)));
                }
            }
        }

    }

}

/* Перезагрузка списка задач в интерфейсе */
void controller_mainWindow::reloadTaskList(QString saveFlag)
{
    qDebug() << "Функция reloadTaskList запущена (контроллер) " << saveFlag;

    /** Обрабатываем календарь */

    connect (this, SIGNAL(s_setClearCalendar()), model, SLOT(setClearCalendar()));
    emit s_setClearCalendar();
    disconnect (this, SIGNAL(s_setClearCalendar()), model, SLOT(setClearCalendar()));

    QString currTaskDate, trueDate;
    QDate   markDate;

    /** Перебираем данные в структуре списка задач */
    for(int i=0; i<tList.taskDate.count(); i++)
    {
        currTaskDate = tList.taskDate[i].toString();
       // qDebug() << "Сравнение месяца и года в календаре и из файла. В файле " << currTaskDate.mid(3,7) << " В календаре " << calendarDate.toString("MM_yyyy");
        /** Если задача за текущий месяц и год */
        if (currTaskDate.mid(3,7) == calendarDate.toString("MM_yyyy"))
        {
            /** Помечаем дату этой задачи на календаре жирным шрифтом */
            trueDate = currTaskDate.mid(0,2);
            markDate = QDate::fromString(currTaskDate, "dd_MM_yyyy");
            //qDebug() << "markDate " << markDate;
            connect (this, SIGNAL(s_setDayColor(QDate)), model, SLOT(setDayColor(QDate)));
            emit s_setDayColor(markDate);
            disconnect (this, SIGNAL(s_setDayColor(QDate)), model, SLOT(setDayColor(QDate)));
        }
    }

    if (saveFlag == "0")
    {
        saveDay();
    }

    /** Запускаем процедуру загрузки списка задач в модели */
    connect( this, SIGNAL(s_reloadTaskList()), model, SLOT(reloadTaskList()));
    emit s_reloadTaskList();
    disconnect( this, SIGNAL(s_reloadTaskList()),model, SLOT(reloadTaskList()));

    /** Запускаем подсчёт времени и суммы, потраченной за выбранный в данный момент в календаре день */
    calcDayTime();
}

/* Открытие диалога создания задачи */
void controller_mainWindow::open_createTaskDialog()
{
    qDebug() << "Функция open_createTaskDialog запустилась (контроллер)" ;
    /** Открытваем окно для ввода короткого наименования новой задачи */

    connect(dct, SIGNAL(nameEntered(QString)), this, SLOT(addTask(QString)));

    dct->setWindowTitle("Новая задача");

    dct->labelText = "Введите название задачи...";
    dct->init_windowParams();
}

/* Добавление информации о задаче */
void controller_mainWindow::addTask(QString taskName)
{
    qDebug() << "Функция addTask запустилась (контроллер)";
    int newIndex = tList.taskIndex.count();

    tList.taskDate       .insert(newIndex, QVariant(calendarDate.toString("dd_MM_yyyy")));
    tList.taskIndex      .insert(newIndex, QVariant(QString::number(newIndex)));
    tList.taskName       .insert(newIndex, QVariant(taskName));
    tList.taskFullText   .insert(newIndex, QVariant(""));
    tList.taskHour       .insert(newIndex, QVariant("0"));
    tList.taskKop        .insert(newIndex, QVariant("0"));
    tList.taskMinute     .insert(newIndex, QVariant("0"));
    tList.taskPrice      .insert(newIndex, QVariant("0 рублей 00 копеек"));
    tList.taskRuble      .insert(newIndex, QVariant("0"));
    tList.taskSecond     .insert(newIndex, QVariant("0"));
    tList.taskTime       .insert(newIndex, QVariant("00:00:00"));
    tList.completeFlag   .insert(newIndex, QVariant("0"));
    saveDay();
    setDay();

    disconnect (dct, SIGNAL(nameEntered(QString)), this, SLOT(addTask(QString)));

    reloadTaskList("1");
}

/* Запуск таймера задачи */
void controller_mainWindow::startTaskTimer()
{
    qDebug() << "Функция startTaskTimer запустилась (контроллер)";
    timer->start(1000);
    connect(timer,SIGNAL(timeout()), this, SLOT(setTime()));
}

/* Остановка таймера задачи */
void controller_mainWindow::stopTaskTimer()
{
    qDebug() << "Функция stopTaskTimer запустилась (контроллер)";
    timer->stop();
    disconnect(timer,SIGNAL(timeout()), this, SLOT(setTime()));
    qDebug() << "Рабочий каталог " << *workDir;
    if (workDir != QDir::home().path())
    saveDay();
}

/* Посчитать очередную секунду */
void controller_mainWindow::setTime()
{
    qDebug() << "Функция setTime запустилась (контроллер)";

    //currIndexNum = indexNumList[currTaskNum].toInt();
    qDebug() << "Для очередной секунды наименование задачи " << tList.taskName[currIndexNum] << " Номер индекса " << currIndexNum << " Номер задачи на экране " << currTaskNum;

    /** Считаем обновлённые часы, минуты и секунды */

    int secondCount = tList.taskSecond   [currIndexNum].toInt();
    int minuteCount = tList.taskMinute   [currIndexNum].toInt();
    int hourCount   = tList.taskHour     [currIndexNum].toInt();

    secondCount += 1;
    if (secondCount > 59)
    {
        secondCount = secondCount-60;
        minuteCount +=1;
    }
    if (minuteCount > 59)
    {
        minuteCount = minuteCount-60;
        hourCount   +=  1;
    }

    /** Сохраняем обновлённые таймеры */
    tList.taskSecond   [currIndexNum] = secondCount  ;
    tList.taskMinute   [currIndexNum] = minuteCount  ;
    tList.taskHour     [currIndexNum] = hourCount    ;

    /** Формируем строку таймера с корректными значениями */
    QString secondCountSt   = QString::number(secondCount   );
    QString minuteCountSt   = QString::number(minuteCount   );
    QString hourCountSt     = QString::number(hourCount     );

    if(secondCount < 10){secondCountSt    = "0"+secondCountSt;}
    if(minuteCount < 10){minuteCountSt    = "0"+minuteCountSt;}
    if(hourCount   < 10){hourCountSt      = "0"+hourCountSt;  }

    tList.taskTime[currIndexNum] = hourCountSt+":"+minuteCountSt+":"+secondCountSt;

    /** Задаём текущее время задачи */
    connect(this, SIGNAL(s_setTaskTime()), model, SLOT(setTaskTime()));
    emit s_setTaskTime();
    disconnect(this, SIGNAL(s_setTaskTime()), model, SLOT(setTaskTime()));

    calcTaskPrice();
}

/* Обновить значение цены выполнения задачи */
void controller_mainWindow::calcTaskPrice()
{
    qDebug() << "Функция calcTaskPrice запустилась (контроллер)";

    /** Подсчитываем стоимость выполнения выбранной задачи */
    double curr_secondKopPrice = curr_hourPrice.toDouble()/60/60*100;
    double curr_minuteKopPrice = curr_hourPrice.toDouble()/60*100;
    double curr_hourKopPrice   = curr_hourPrice.toDouble()*100;

    double secPriceSumm    = curr_secondKopPrice   *tList.taskSecond[currIndexNum].toDouble();
    double minPriceSumm    = curr_minuteKopPrice   *tList.taskMinute[currIndexNum].toDouble();
    double hourPriceSumm   = curr_hourKopPrice     *tList.taskHour  [currIndexNum].toDouble();

    double kopSumm = secPriceSumm+minPriceSumm+hourPriceSumm+rublCount*100+kopCount;

    double  d_rublSumm = floor(kopSumm/100);
    double  d_kopSumm  = kopSumm - d_rublSumm*100;
    if (d_kopSumm > 99)
    {
        d_kopSumm   = abs(d_kopSumm     - 100)  ;
        d_rublSumm  = d_rublSumm    + 1         ;
    }

    tList.taskRuble  [currIndexNum]    = QString::number(d_rublSumm, 'f', 0);
    tList.taskKop    [currIndexNum]    = QString::number(d_kopSumm, 'f', 0);

    if(tList.taskKop [currIndexNum].toInt()<10){tList.taskKop[currIndexNum] = "0"+tList.taskKop[currIndexNum].toString();}

    tList.taskPrice[currIndexNum] = tList.taskRuble[currIndexNum].toString()+" рублей "+tList.taskKop[currIndexNum].toString()+" копеек";

    /** Задаём текущую стоимость задачи  */
    connect(this, SIGNAL(s_setTaskPrice()), model, SLOT(setTaskPrice()));
    emit s_setTaskPrice();
    disconnect(this, SIGNAL(s_setTaskPrice()), model, SLOT(setTaskPrice()));

    calcDayTime();
}

/* Считаем время, потраченное за день */
void controller_mainWindow::calcDayTime()
{
    qDebug() << "Функция calcDayTime запустилась (контроллер)";

    int summDaySecond  = 0;
    int summDayMinute  = 0;
    int summDayHour    = 0;

    int summAllSecond  = 0;
    int summAllMinute  = 0;
    int summAllHour    = 0;

    /** Перебираем задачи для подсчёта общего времени */
    for(int i=0; i<tList.taskHour.count(); i++)
    {
        /** Если задача выполняется в текущий выбранный день, считаем ещё и время за день */
        if (tList.taskDate[i].toString() == calendarDate.toString("dd_MM_yyyy"))
        {
            summDaySecond += tList.taskSecond[i].toInt();
            if (summDaySecond > 59)
            {
                summDaySecond  = summDaySecond - 60;
                summDayMinute += 1;
            }
            summDayMinute += tList.taskMinute[i].toInt();
            if (summDayMinute > 59)
            {
                summDayMinute   = summDayMinute - 60;
                summDayHour    += 1;
            }
            summDayHour   += tList.taskHour[i].toInt();
        }

        summAllSecond += tList.taskSecond[i].toInt();
        if (summAllSecond > 59)
        {
            summAllSecond  = summAllSecond - 60;
            summAllMinute += 1;
        }
        summAllMinute += tList.taskMinute[i].toInt();
        if (summAllMinute > 59)
        {
            summAllMinute   = summAllMinute - 60;
            summAllHour    += 1;
        }
        summAllHour   += tList.taskHour[i].toInt();
    }

    /** Подсчёт суммарно потраченных денег на основании суммарного времени за весь проект */
    QString summAllHourSt, summAllMinuteSt, summAllSecondSt;
    if (summAllHour    <10)  {summAllHourSt     = "0"+QString::number(summAllHour);    }else{summAllHourSt = QString::number(summAllHour);       }
    if (summAllMinute  <10)  {summAllMinuteSt   = "0"+QString::number(summAllMinute);  }else{summAllMinuteSt = QString::number(summAllMinute);   }
    if (summAllSecond  <10)  {summAllSecondSt   = "0"+QString::number(summAllSecond);  }else{summAllSecondSt = QString::number(summAllSecond);   }

    allTime = summAllHourSt+":"+summAllMinuteSt+":"+summAllSecondSt;

    /** Подсчёт суммарно потраченных денег на основании суммарного времени за день */
    QString summDayHourSt, summDayMinuteSt, summDaySecondSt;
    if (summDayHour    <10)  {summDayHourSt     = "0"+QString::number(summDayHour);    }else{summDayHourSt = QString::number(summDayHour);       }
    if (summDayMinute  <10)  {summDayMinuteSt   = "0"+QString::number(summDayMinute);  }else{summDayMinuteSt = QString::number(summDayMinute);   }
    if (summDaySecond  <10)  {summDaySecondSt   = "0"+QString::number(summDaySecond);  }else{summDaySecondSt = QString::number(summDaySecond);   }

    dayTime = summDayHourSt+":"+summDayMinuteSt+":"+summDaySecondSt;

    /** Задаём потраченное время за день  */
    connect(this, SIGNAL(s_setTaskDayTime()), model, SLOT(setDayTime()));
    emit s_setTaskDayTime();
    disconnect(this, SIGNAL(s_setTaskDayTime()), model, SLOT(setDayTime()));

    calcDayPrice();
}

/* Считаем деньги, потраченные за день */
void controller_mainWindow::calcDayPrice()
{
    qDebug() << "Функция calcDayPrice запустилась (контроллер)";
    int summAllSecondCount = 0;
    int summAllMinuteCount = 0;
    int summAllHoreCount   = 0;

    int summDaySecondCount = 0;
    int summDayMinuteCount = 0;
    int summDayHoreCount   = 0;

    summAllSecondCount = allTime.mid(6,2).toInt();
    summAllMinuteCount = allTime.mid(3,2).toInt();
    summAllHoreCount   = allTime.mid(0,2).toInt();

    summDaySecondCount = dayTime.mid(6,2).toInt();
    summDayMinuteCount = dayTime.mid(3,2).toInt();
    summDayHoreCount   = dayTime.mid(0,2).toInt();

    /** Подстчитываем фактическую стоимость секунды, минуты и часа */
    double curr_secondKopPrice = curr_hourPrice.toDouble()/60/60*100;
    double curr_minuteKopPrice = curr_hourPrice.toDouble()/60*100   ;
    double curr_hourKopPrice   = curr_hourPrice.toDouble()*100      ;

    /** Подсчитываем стоимость суммарного потраченного времени за день*/
    double secDayPriceSumm    = curr_secondKopPrice   *summDaySecondCount ;
    double minDayPriceSumm    = curr_minuteKopPrice   *summDayMinuteCount ;
    double hourDayPriceSumm   = curr_hourKopPrice     *summDayHoreCount   ;

    double kopDaySumm = secDayPriceSumm+minDayPriceSumm+hourDayPriceSumm+rublCount*100+kopCount;

    QString displDayRublSumm    = QString::number(floor(kopDaySumm/100), 'f', 0);
    QString displDayKopSumm     = QString::number(kopDaySumm - displDayRublSumm.toDouble()*100, 'f', 0);

    if(displDayKopSumm.toInt()<10){displDayKopSumm = "0"+displDayKopSumm;}
    dayPrice = displDayRublSumm+" рублей "+displDayKopSumm+" копеек";

    /** Подсчитываем стоимость суммарного потраченного времени за проект*/
    double secAllPriceSumm    = curr_secondKopPrice   *summAllSecondCount ;
    double minAllPriceSumm    = curr_minuteKopPrice   *summAllMinuteCount ;
    double hourAllPriceSumm   = curr_hourKopPrice     *summAllHoreCount   ;

    double kopAllSumm = secAllPriceSumm+minAllPriceSumm+hourAllPriceSumm+rublCount*100+kopCount;

    QString displAllRublSumm    = QString::number(floor(kopAllSumm/100), 'f', 0);
    QString displAllKopSumm     = QString::number(kopAllSumm - displAllRublSumm.toDouble()*100, 'f', 0);

    if(displAllKopSumm.toInt()<10){displAllKopSumm = "0"+displAllKopSumm;}
    allPrice = displAllRublSumm+" рублей "+displAllKopSumm+" копеек";

    /** Задаём потраченные деньги за день  */
    connect(this, SIGNAL(s_setTaskDayPrice()), model, SLOT(setDayPrice()));
    emit s_setTaskDayPrice();
    disconnect(this, SIGNAL(s_setTaskDayPrice()), model, SLOT(setDayPrice()));

}

/* Открываем окно подтвержения удаления задачи */
void controller_mainWindow::open_removeTaskDialog()
{
    qDebug() << "Функция open_removeTaskDialog запустилась (контроллер)";
    connect(di, SIGNAL(s_remove()), this, SLOT(removeCurrTask()));
    di->setWindowTitle("Требуется подтверждение.");
    di->text    = "Вы уверены что хотите удалить задачу '"+tList.taskName[currIndexNum].toString()+"' ?";
    di->title   = "Вы уверены?";
    di->setInfo();
    di->open();
}

/* Удаляем задачу */
void controller_mainWindow::removeCurrTask()
{
    qDebug() << "Функция removeCurrTask запустилась (контроллер)";
    disconnect(di, SIGNAL(s_remove()), this, SLOT(removeCurrTask()));

    qDebug() << "Количество задач в списке до удаления " << tList.taskName.count();
    qDebug() << "Удаляемая задача " << tList.taskName[currIndexNum];

    /** Удаляем из структуры списка задач упоминание о данной задаче */
    tList.taskFullText   .removeAt(currIndexNum);
    tList.taskHour       .removeAt(currIndexNum);
    tList.taskKop        .removeAt(currIndexNum);
    tList.taskMinute     .removeAt(currIndexNum);
    tList.taskName       .removeAt(currIndexNum);
    tList.taskPrice      .removeAt(currIndexNum);
    tList.taskRuble      .removeAt(currIndexNum);
    tList.taskSecond     .removeAt(currIndexNum);
    tList.taskTime       .removeAt(currIndexNum);
    tList.taskDate       .removeAt(currIndexNum);
    tList.taskIndex      .removeAt(currIndexNum);
    tList.completeFlag   .removeAt(currIndexNum);

    indexNumList         .removeAt(currTaskNum);
    taskNumList          .removeAt(currTaskNum);

    qDebug() << "Количество задач в списке после удаления " << tList.taskName.count();

    /** Запускаем функцию удаления задачи из строки интерфейса */
    connect(this, SIGNAL(s_removeCurrTask()), model, SLOT(removeCurrTask()));
    emit s_removeCurrTask();
    disconnect(this, SIGNAL(s_removeCurrTask()), model, SLOT(removeCurrTask()));

    saveDay();
    setDay();
}

/* Обрабатываем смену рабочего дня с помощью календаря */
void controller_mainWindow::setDay(int flag)
{
    qDebug() << "Функция controller_mainWindow::setDay() запустилась (контроллер)" << flag;

    timer->stop()   ;
    currTaskNum = 0 ;
    init_window(flag)  ;
}

/* Сохранение данных за день для данного проекта */
void controller_mainWindow::saveDay()
{
    qDebug() << "Функция controller_mainWindow::saveDay() запустилась (контроллер) " ;
    writeToJSON();
}

/* Открытваем окно настроек проекта */
void controller_mainWindow::open_projSettings(bool tr)
{
    qDebug() << "Функция controller_mainWindow::open_projSettings(bool tr) запустилась (контроллер) " ;
    dps->cps.pList      = pList     ;
    dps->cps.workDir    = workDir   ;
    dps->parentObj      = this      ;
    dps->cps.model      = dps       ;
    dps->cps.init_window();
}

/* Сохраняем данные проекта */
void controller_mainWindow::setProjParams()
{
    qDebug() << "Функция setProjParams запустилась (контроллер) " ;
    dps->cps.pList      = pList     ;
    dps->cps.workDir    = workDir   ;
    curr_hourPrice = pList->projPriceSt;
    setDay();
}

/* Подготовка параметров и открытие окна ввода звонка */
void controller_mainWindow::open_createCallDialog(bool tr)
{
    qDebug() << "Функция open_createCallDialog запустилась (контроллер) " << *workDir;
    Dialog_createCall *dcc = new Dialog_createCall;
    dcc->ccc.tList      = tList;
    dcc->ccc.workDir    = workDir;
    dcc->init_window();
}

/* Подготовка параметров и открытие окна просмотра звонков */
void controller_mainWindow::open_viewCallDialog(bool tr)
{
    qDebug() << "Функция open_viewCallDialog запустилась (контроллер) " << workDir ;
    Dialog_viewCallList *dvcl   = new Dialog_viewCallList   ;
    dvcl->cvcl.cList            = cList                     ;
    dvcl->cvcl.pList            = pList                     ;
    dvcl->cvcl.tList            = tList                     ;
    dvcl->cvcl.workDir          = workDir                   ;

    dvcl->init_windowParams();
}

/* Подготовка параметров и открытие окна поиска */
void controller_mainWindow::open_dialogFind(bool tr)
{
    qDebug() << "Функция open_dialogFind запустилась (контроллер) ";
    Dialog_find *df = new Dialog_find;
    df->cf.workDir = workDir;
    df->setWindowTitle("Поиск");
    df->init_window();
}

/** Открытие параметров проекта и их запись в глобальные переменные */
void controller_mainWindow::open_project()
{
    qDebug() << "Функция controller_mainWindow::open_project() запустилась";
    if (dps->caller == "createNewProj")
    {
        workDir = dps->cps.workDir;
        dps->caller = "";
    }
    readProjFromFile();
    disconnect(dps, SIGNAL(s_setProjParams()), model, SLOT(open_project()));

    if (j_dataArray.count() > 0)
    {
        pList->projNameSt        = j_dataArray[0].toArray()[0].toString();
        pList->projPriceSt       = j_dataArray[0].toArray()[1].toString();
        pList->projDescriptionSt = j_dataArray[0].toArray()[2].toString();
        pList->projContactNameSt = j_dataArray[0].toArray()[3].toString();
        pList->projPhoneSt       = j_dataArray[0].toArray()[4].toString();
        pList->projEmailSt       = j_dataArray[0].toArray()[5].toString();

        projList << j_dataArray[0].toArray()[0].toString();

        curr_hourPrice = pList->projPriceSt;
    }
}

/* Обработка нажатия кнопки создания проекта */
void controller_mainWindow::create_project()
{
    qDebug() << "Функция controller_mainWindow::create_project() запустилась (контроллер)";
    connect(dps, SIGNAL(s_setProjParams()), model, SLOT(open_project()));

    dps->cps.pList  = pList;
    dps->cps.workDir = workDir;
    dps->init_window("createNewProj");

}

/* Диалог подтверждения переноса задачи на указанную дату */
void controller_mainWindow::open_confirmDialog(QString text)
{
    qDebug() << "Функция open_confirmDialog запустилась (контроллер)";
    Dialog_info *di = new Dialog_info;
    //connect(di, Dialog_info::s_taskMove, this, move_task);
    di->setWindowTitle("Вы уверены? ");
    di->text = text;
    di->setInfo();
    di->show();
}

/* Перенос задачи на новую дату */
void controller_mainWindow::move_task()
{
    qDebug() << "Функция move_task запустилась (контроллер)";
    //disconnect(di, Dialog_info::s_taskMove, this, move_task);

}

/* Сохранение всех данных */
void controller_mainWindow::save_project(bool tr)
{
    qDebug() << "Функция save_project запустилась (контроллер)";
    saveDay();
}

/* Закрытие проекта */
void controller_mainWindow::close_project(int index)
{
    qDebug() << "Функция controller_mainWindow::close_project(int index) запустилась (контроллер)";

    saveDay();
    projList.removeAt(index);
}
