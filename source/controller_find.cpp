#include "controller_find.h"

controller_find::controller_find()
{

}

/* Получение списка папок проектов */
void controller_find::get_projList()
{
    qDebug() << "Функция controller_find::get_projList() запутилась (контроллер) " << *workDir;

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

/* Алгоритм поиска по критериям из формы */
void controller_find::process_search()
{
    qDebug() << "Функция process_search запутилась (контроллер)";
    fList.date          .clear();
    fList.topic         .clear();
    fList.time          .clear();
    fList.description   .clear();
    fList.type          .clear();
    fList.projectName   .clear();



    if (allProject)
    {
        for (int i=0; i<qsl.count(); i++)
        {
            QDir qd(*workDir);
            qd.cd("..");

            QString tempWorkDir = *workDir;
            *workDir = qd.path()+"/"+ qsl[i];
            qDebug() << "Очередной каталог проекта " << *workDir;
            readFromFile    ()      ;
            *workDir = tempWorkDir;
            /** Добавляем в структуру taskList задачи за весь проект */
            for (int j=0; j<j_dataArray.count(); j++)
            {
                fList.date          << j_dataArray[j].toArray()[0].toString();
                fList.topic         << j_dataArray[j].toArray()[2].toString();
                fList.time          << j_dataArray[j].toArray()[3].toString();
                fList.description   << j_dataArray[j].toArray()[5].toString();
                fList.type          << "Задача";
                fList.projectName   << qsl[i];
            }

            readCallFromFile();

            for (int j=0; j<j_dataArray.count(); j++)
            {
                fList.date        << j_dataArray[j].toArray()[0].toString();
                fList.time        << j_dataArray[j].toArray()[1].toString();
                fList.topic       << j_dataArray[j].toArray()[4].toString();
                fList.description << j_dataArray[j].toArray()[5].toString();
                fList.type        << "Звонок";
                fList.projectName << qsl[i];
            }
        }
    }
    else
    {
        QDir qd(*workDir);
        qd.cd("..");
        QString tempWorkDir = *workDir;
        *workDir = qd.path()+"/"+ projectName;

        qDebug() << "Очередной каталог проекта " << *workDir;
        readFromFile    ()      ;
        *workDir = tempWorkDir;
        /** Добавляем в структуру taskList задачи за весь проект */
        qDebug() << "Размер массива " << j_dataArray.count();
        for (int i=0; i<j_dataArray.count(); i++)
        {
            qDebug() << "Итерация " << i;
            fList.date          << j_dataArray[i].toArray()[0].toString();
            fList.topic         << j_dataArray[i].toArray()[2].toString();
            fList.time          << j_dataArray[i].toArray()[3].toString();
            fList.description   << j_dataArray[i].toArray()[5].toString();
            fList.type          << "task";
            fList.projectName   << projectName;
        }

        readCallFromFile();

        for (int i=0; i<j_dataArray.count(); i++)
        {
            fList.date        << j_dataArray[i].toArray()[0].toString();
            fList.time        << j_dataArray[i].toArray()[1].toString();
            fList.topic       << j_dataArray[i].toArray()[4].toString();
            fList.description << j_dataArray[i].toArray()[5].toString();
            fList.type        << "call";
            fList.projectName << projectName;
        }
    }

    if (allDate == false)
    {
        int fListCount = fList.topic.count();
        int j = 0;
        for (int i=0; i<fListCount; i++)
        {
            qDebug() << "Дата начала " << QDate::fromString(fList.date[j].toString(),"dd_MM_yyyy") << " Дата в выборке " << dateBegin;
            if(QDate::fromString(fList.date[j].toString(),"dd_MM_yyyy") < dateBegin || QDate::fromString(fList.date[j].toString(),"dd_MM_yyyy") > dateEnd)
            {
                fList.date          .removeAt(j);
                fList.topic         .removeAt(j);
                fList.time          .removeAt(j);
                fList.description   .removeAt(j);
                fList.type          .removeAt(j);
                fList.projectName   .removeAt(j);
            }
            else
            {
                j++;
            }
        }
    }

    if (allTasks == false)
    {
        int fListCount = fList.topic.count();
        int j = 0;
        for (int i=0; i<fListCount; i++)
        {
            if(fList.type[j].toString() == "task")
            {
                fList.date          .removeAt(j);
                fList.topic         .removeAt(j);
                fList.time          .removeAt(j);
                fList.description   .removeAt(j);
                fList.type          .removeAt(j);
                fList.projectName   .removeAt(j);
            }
            else
            {
                j++;
            }
        }
    }

    if (allCalls == false)
    {
        int fListCount = fList.topic.count();
        int j = 0;
        for (int i=0; i<fListCount; i++)
        {
            if(fList.type[j].toString() == "call")
            {
                fList.date          .removeAt(j);
                fList.topic         .removeAt(j);
                fList.time          .removeAt(j);
                fList.description   .removeAt(j);
                fList.type          .removeAt(j);
                fList.projectName   .removeAt(j);
            }
            else
            {
                j++;
            }
        }
    }
    if (setSubString)
    {
        int fListCount = fList.topic.count();
        int j = 0;
        for (int i=0; i<fListCount; i++)
        {
            if (fList.topic[j].toString().contains(subString) || fList.description[j].toString().contains(subString))
            {
                j++;
            }
            else
            {
                qDebug() << "Удаляем задачу " << fList.topic[j].toString() << " Наличие данных в заголовке " << fList.topic[j].toString().contains(subString) << " Наличие данных в описании " << fList.topic[j].toString().contains(subString);
                fList.date          .removeAt(j);
                fList.topic         .removeAt(j);
                fList.time          .removeAt(j);
                fList.description   .removeAt(j);
                fList.type          .removeAt(j);
                fList.projectName   .removeAt(j);

            }
        }
    }


}
