#include "controller_projectsettings.h"

controller_projectSettings::controller_projectSettings()
{

}

/** Получение информации о проекте в случае, если он открыт */
void controller_projectSettings::init_window()
{
    qDebug() << "Функция controller_projectSettings::init_window() запустилась";
    /** Читаем файл проекта и наполняем структуру реквизитов проекта данными */
    readProjFromFile();
    QString caller = "createNewProj";
    if (j_dataArray.count() > 0)
    {
        pList->projNameSt        = j_dataArray[0].toArray()[0].toString();
        pList->projPriceSt       = j_dataArray[0].toArray()[1].toString();
        pList->projDescriptionSt = j_dataArray[0].toArray()[2].toString();
        pList->projContactNameSt = j_dataArray[0].toArray()[3].toString();
        pList->projPhoneSt       = j_dataArray[0].toArray()[4].toString();
        pList->projEmailSt       = j_dataArray[0].toArray()[5].toString();

        curr_hourPrice = pList->projPriceSt;
        caller = "openProject";
    }
    connect(this, SIGNAL(s_paramsReady(QString)), model, SLOT(init_window(QString)));
    emit s_paramsReady(caller);
    disconnect(this, SIGNAL(s_paramsReady(QString)), model, SLOT(init_window(QString)));
}

/* Создание каталога нового проекта */
void controller_projectSettings::create_newProject()
{
    qDebug() << "Функция controller_projectSettings::create_newProject() запустилась";

    QDir qd(*workDir);
    if (qd.exists("PRJ.dat"))
    {
        qd.cdUp();
    }
    qd.mkdir(pList->projNameSt);
    qd.cd(pList->projNameSt);
    *workDir = qd.path();

    qDebug() << "Созданный рабочий каталог "<< *workDir;
    writeProjToJSON();
}



