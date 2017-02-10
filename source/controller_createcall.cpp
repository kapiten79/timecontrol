#include "controller_createcall.h"

controller_createCall::controller_createCall()
{
    setTodayDateTime();
    //readCallFromFile();
    qDebug() << "Прочлось";

}

/* Задаётся текущая дата и время */
void controller_createCall::setTodayDateTime()
{
    /** Получаем все необходимые значения текущей даты и времени */
    todayDate   = QDate::currentDate()  ;
    calendarDate= QDate::currentDate()  ;
    currTime    = QTime::currentTime()  ;
}


/* Получение списка папок проектов */
void controller_createCall::get_projList()
{
    qDebug() << "Функция controller_createCall::get_projList() запусилась (контроллер)";

    QStringList qsl;
    qDebug() << *workDir;
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

/* Сохранение звонка (начало) */
void controller_createCall::press_ok(bool newProj, QString projName)
{
    qDebug() << "Функция press_ok запусилась";
    /** Если звонок относится к новому проекту, открываем окно создания нового проекта */
    if (newProj == true)
    {
        connect(dps, SIGNAL(s_setProjParams(QString)), this, SLOT(saveCall(QString)));
        dps->init_window("createCall");
    }
    /** Если звонок привязан к существующему проекту, сохраняем его в базе звонков в каталоге проекта */
    else
    {
        saveCall(projName);
    }
}

/* Сохранение звонка (сохранение данных) */
void controller_createCall::saveCall(QString projName)
{
    qDebug() << "Функция controller_createCall::saveCall(QString projName) запусилась ";
    cList.callProject << projName;
    disconnect(dps, SIGNAL(s_setProjParams(QString)), this, SLOT(saveCall(QString)));
    pList->projNameSt = projName;
    //workDir = QDir::currentPath()+"/Project/"+projName;
    writeCallToJSON();
}

void controller_createCall::get_oldCalls()
{
    qDebug() << "Функция get_oldCalls запусилась " << cList.callDate.count();
    readCallFromFile();
    cList.callDate          .clear();
    cList.callTime          .clear();
    cList.callIndex         .clear();
    cList.callProject       .clear();
    cList.callTopics        .clear();
    cList.callDescription   .clear();

    for (int i=0; i<j_dataArray.count(); i++)
    {
        cList.callDate        << j_dataArray[i].toArray()[0].toString();
        cList.callTime        << j_dataArray[i].toArray()[1].toString();
        cList.callIndex       << j_dataArray[i].toArray()[2].toString();
        cList.callProject     << j_dataArray[i].toArray()[3].toString();
        cList.callTopics      << j_dataArray[i].toArray()[4].toString();
        cList.callDescription << j_dataArray[i].toArray()[5].toString();
    }
}
