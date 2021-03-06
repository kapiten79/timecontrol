#include "controller_main.h"

controller_main::controller_main()
{
    /** Проверяем, создавалась ли ранее база данных для этого экземпляра программы. */
    if (!sdb.contains())
    {
        sdb = QSqlDatabase::addDatabase("QSQLITE");
        sdb.setDatabaseName("timeControl.db");
        /** Пытаемся открыть базу данных */

        if (!sdb.open())
        {
            qDebug() << "База данных не открылась" ;
        }
        else
        {
            qDebug() << "База данных открылась" ;
        }

        /** Проверяем, есть ли в базе данных структура или она создана впервые */
        query = "select count(type) from sqlite_master where type = 'table'";
        result = sdb.exec(query);
        int tblCount = 0;
        while (result.next())
        {
            tblCount = result.value(0).toString().toInt();
        }
        /** Если таблиц в базе данных нет, создаём базу данных */
        if (tblCount <= 0)
        {
            qDebug() << "База данных не создана. Генерируем пустую базу данных.";
            query = "CREATE TABLE dict_project   (id_prj int PRIMARY KEY, "
                    "name           text, "
                    "description    text, "
                    "hour_price     int, "
                    "directory      text )";
             result = sdb.exec(query);

             query = "CREATE TABLE settings   (id_set int PRIMARY KEY, "
                     "owner             text, "
                     "version           text, "
                     "serial            text, "
                     "def_hour_price    int, "
                     "directory         text )";
              result = sdb.exec(query);
             qDebug() << "Результат создания базы данных " << result.lastError().text();
        }
        else
        {
            qDebug() << "База данных уже существует. Загружаем информацию о проктах и настройках программы...";
        }

    }
}

/** Получение нового id записи в таблице */
QString controller_main::getLastIndex(QString table)
{
    result = sdb.exec("SELECT count() FROM "+table);
    result.next();
    return result.value(0).toString();
}

/********************** ОБРАБОТКА ФАЙЛОВ РАБОЧЕГИ ДНЯ ********************************/

/* Запись данных о рабочем дне в файл */
void controller_main::saveToFile()
{
    qDebug() << "Функция saveToFile() запустилась. Рабочий каталог (Родительский контроллер)" << *workDir ;

    qDebug() << *workDir+"/DATA.dat";
    QFile file(*workDir+"/DATA.dat");
    qDebug() << file.remove()                       ;
    file.open(QIODevice::WriteOnly)                 ;
    QDataStream out(&file)                          ;
    out << dataToFile                               ;
    file.close()                                    ;
}

/* Чтение данных о рабочем дне из файла */
void controller_main::readFromFile()
{
    qDebug() << "Функция readFromFile() запустилась (Родительский контроллер) ";

    int dataArray_count = j_dataArray.count();
    for (int i=0; i<dataArray_count; i++)
    {
        j_dataArray.removeAt(0);
    }
    dataFromFile.clear();

    if (workDir->length() > 0)
    {
        qDebug() << *workDir+"/DATA.dat";
        QFile file(*workDir+"/DATA.dat");
        if (file.exists())
        {
            if(file.open(QIODevice::ReadOnly))
            {
                qDebug() << "Файл открылся"     ;
                QDataStream in(&file)           ;
                in       >> dataFromFile        ;
                file.close();
                readFromJSON();
            }
            else
            {
                qDebug() << "Файл не открылся";
            }

        }
        else
        {
            qDebug() << "Файла нету";
        }
    }

}

/* Обработка данных о рабочем дне, полученных из файла */
void controller_main::readFromJSON()
{
    qDebug() << "Функция readFromJSON() запустилась (Родительский контроллер)";
    QJsonParseError p_j_error;
    QJsonDocument   d_data = QJsonDocument::fromBinaryData(dataFromFile);

    qDebug() << "Информация, полученная из файла " << d_data;

    j_dataArray = d_data.array();
    qDebug() << "Ошибка парсинга json " << p_j_error.errorString();
}

/* Перевод данных о рабочем дне в формат JSON для последующей записи в файл */
void controller_main::writeToJSON()
{
    qDebug() << "Функция writeToJSON() запустилась";

    QVariantList newTaskQv;
    QJsonArray firstLevel;
    QJsonArray secondLevel;

    for (int i=0; i<tList.taskName.count(); i++)
    {
        newTaskQv.clear();
        newTaskQv << tList.taskDate       [i].toString();
        newTaskQv << tList.taskIndex      [i].toString();
        newTaskQv << tList.taskName       [i].toString();
        newTaskQv << tList.taskTime       [i].toString();
        newTaskQv << tList.taskPrice      [i].toString();
        newTaskQv << tList.taskFullText   [i].toString();
        newTaskQv << tList.completeFlag   [i].toString();
        secondLevel = QJsonArray::fromVariantList(newTaskQv);
        firstLevel.insert(firstLevel.count(),secondLevel);
    }
    QJsonDocument arrTaskDoc(firstLevel);

    qDebug() << "Информация, пишущаяся в файл " << arrTaskDoc;

    dataToFile = arrTaskDoc.toBinaryData();
    saveToFile();
}

/******************** ОБРАБОТКА ФАЙЛОВ ПРОЕКТОВ **********************************/

/* Запись данных о рабочем дне в файл */
void controller_main::saveProjToFile()
{
    qDebug() << "Функция controller_main::saveProjToFile() запустилась. (Родительский контроллер) Рабочий каталог " << *workDir;

    qDebug() << *workDir+"/PRJ.dat";
    QFile file(*workDir+"/PRJ.dat");
    qDebug() << file.remove()                       ;
    file.open(QIODevice::WriteOnly)                 ;
    QDataStream out(&file)                          ;
    out << dataToFile                               ;
    file.close()                                    ;
}


void controller_main::readProjFromFile()
{
    qDebug() << "Функция readProjFromFile() запустилась";

    /** Очищаем JSON массив с данными проекта */
    int dataArray_count = j_dataArray.count();
    for (int i=0; i<dataArray_count; i++)
    {
        j_dataArray.removeAt(0);
    }
    dataFromFile.clear();

    /** Если насменование рабочего каталога не пустое */
    if (workDir->length() > 0)
    {
        qDebug() << *workDir+"/PRJ.dat";

        /** Открываем файл проекта */
        QFile file(*workDir+"/PRJ.dat");
        if (file.exists())
        {
            if(file.open(QIODevice::ReadOnly))
            {
                qDebug() << "Файл открылся";

                QDataStream in(&file)       ;
                in          >> dataFromFile ;
                file.close();

                readProjFromJSON();
            }
            else
            {
                qDebug() << "Файл не открылся";
            }

        }
        else
        {
            qDebug() << "Файла нету";
        }
    }

}


/* Обработка данных из файла проекта */
void controller_main::readProjFromJSON()
{
    qDebug() << "Функция readProjFromJSON() запустилась";
    QJsonParseError p_j_error;
    QJsonDocument   d_data  = QJsonDocument::fromBinaryData(dataFromFile);
    j_dataArray             = d_data.array();

    qDebug() << "Ошибка парсинга json " << p_j_error.errorString();
}

/* Подготовка данных к записи в файл проекта */
void controller_main::writeProjToJSON()
{
    qDebug() << "Функция writeProjToJSON() запустилась";

    QVariantList projQv;
    QJsonArray firstLevel;
    QJsonArray secondLevel;

    projQv.clear();
    projQv << pList->projNameSt;
    projQv << pList->projPriceSt;
    projQv << pList->projDescriptionSt;
    projQv << pList->projContactNameSt;
    projQv << pList->projPhoneSt;
    projQv << pList->projEmailSt;
    secondLevel = QJsonArray::fromVariantList(projQv);
    firstLevel.insert(0,secondLevel);

    QJsonDocument arrTaskDoc(firstLevel);

    qDebug() << arrTaskDoc;

    dataToFile = arrTaskDoc.toBinaryData();
    saveProjToFile();
}

/******************** ОБРАБОТКА ФАЙЛОВ ЗВОНКОВ **********************************/

/* Запись данных о рабочем дне в файл */
void controller_main::saveCallToFile()
{
    qDebug() << "Текущий каталог проекта " << *workDir;
    qDebug() << "Функция saveCallToFile() запустилась. (Родительский контроллер) Рабочий каталог " << *workDir;


    if (workDir->length() > 0)
    {
        QFile file(*workDir+"/CALL.dat");
        qDebug() << file.remove()                       ;
        file.open(QIODevice::WriteOnly)                 ;
        QDataStream out(&file)                          ;
        out << dataToFile                               ;
        file.close()                                    ;
    }
}


void controller_main::readCallFromFile()
{
    qDebug() << "Функция readCallFromFile() запустилась (Родительский контроллер) " << *workDir+"/CALL.dat";


    int dataArray_count = j_dataArray.count();
    for (int i=0; i<dataArray_count; i++)
    {
        j_dataArray.removeAt(0);
    }
    dataFromFile.clear();

    if (workDir->length() > 0)
    {
        QFile file(*workDir+"/CALL.dat");
        if (file.exists())
        {
            if(file.open(QIODevice::ReadOnly))
            {
                qDebug() << "Файл открылся";

                QDataStream in(&file)       ;
                in          >> dataFromFile ;

                file.close();

                readCallFromJSON();
            }
            else
            {
                qDebug() << "Файл не открылся";
            }

        }
        else
        {
            qDebug() << "Файла нету";
        }
    }
    else
    {
        qDebug() << "Не указан каталог";
    }

}


/* Обработка данных из файла проекта */
void controller_main::readCallFromJSON()
{
    qDebug() << "Функция readCallFromJSON() запустилась (Родительский контроллер)";
    QJsonParseError p_j_error;
    QJsonDocument   d_data  = QJsonDocument::fromBinaryData(dataFromFile);
    j_dataArray             = d_data.array();

    qDebug() << "Информация, полученная из файла по звонкам " << d_data;

    qDebug() << "Ошибка парсинга json " << p_j_error.errorString();
}

/* Подготовка данных к записи в файл проекта */
void controller_main::writeCallToJSON()
{
    qDebug() << "Функция writeCallToJSON() запустилась";

    QVariantList callQv;
    QJsonArray firstLevel;
    QJsonArray secondLevel;


    qDebug() << "Записей в cList " << cList.callDescription.count();
    for (int i=0; i<cList.callDescription.count(); i++)
    {
        callQv.clear();
        callQv << cList.callDate        [i].toString();
        callQv << cList.callTime        [i].toString();
        callQv << cList.callIndex       [i].toString();
        callQv << cList.callProject     [i].toString();
        callQv << cList.callTopics      [i].toString();
        callQv << cList.callDescription [i].toString();
        secondLevel = QJsonArray::fromVariantList(callQv);
        firstLevel.insert(firstLevel.count(),secondLevel);
    }


    QJsonDocument arrTaskDoc(firstLevel);

    qDebug() << arrTaskDoc;

    dataToFile = arrTaskDoc.toBinaryData();
    saveCallToFile();
}


/**************************** КОНТРОЛЬ ЛИЦЕНЗИЙ ************************************/

/* Основная функция, выполняющая контроль за соблюдением лицензионного соглашения */
void controller_main::licenseControl()
{
    qDebug() << "Функция controller_main::licenseControl() запустилась. (Контроллер)" ;
//    query = "lic";
//    select();

}








