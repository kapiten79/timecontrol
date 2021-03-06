#ifndef CONTROLLER_MAIN_H
#define CONTROLLER_MAIN_H

#include <QObject>
#include <QVariantList>
#include <QDebug>
#include <QDateTime>
#include <QDate>
#include <QTime>

#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>

/** Блок устарел */
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>



class controller_main : public QObject
{
public:
    explicit controller_main();

    QSqlDatabase    sdb     ;
    QString         query   ;
    QSqlQuery       result  ;

    /* Переменные дат и времени */

    QDate       calendarDate;
    QDate       todayDate   ;
    QTime       currTime    ;

    struct taskList{
        QVariantList taskDate            ;
        QVariantList taskIndex           ;
        QVariantList taskName            ;
        QVariantList taskTime            ;
        QVariantList taskHour            ;
        QVariantList taskMinute          ;
        QVariantList taskSecond          ;
        QVariantList taskPrice           ;
        QVariantList taskRuble           ;
        QVariantList taskKop             ;
        QVariantList taskFullText        ;
        QVariantList completeFlag        ;
    };

    taskList tList;

    struct projSettings{
        QString projNameSt            ;
        QString projPriceSt           ;
        QString projDescriptionSt     ;
        QString projFlagSt            ;
        QString projContactNameSt     ;
        QString projOrganizationSt    ;
        QString projPhoneSt           ;
        QString projEmailSt           ;
    }pSettings;

    struct projSettings *pList = &pSettings;


    struct callList{
        QVariantList callDate       ;
        QVariantList callTime       ;
        QVariantList callIndex      ;
        QVariantList callProject    ;
        QVariantList callTopics     ;
        QVariantList callDescription;
    };

    callList cList;

    QString     curr_hourPrice = "600"  ;

    QByteArray  dataToFile              ;
    QByteArray  dataFromFile            ;
//    QString     st_curr_date            ;

    QString     *workDir                 ;
    QJsonArray  j_dataArray             ;

    QStringList dayList;

private:


signals:

public slots:
    void saveToFile()   ;
    void readFromFile() ;
    void readFromJSON() ;
    void writeToJSON()  ;

    void saveProjToFile()   ;
    void readProjFromFile() ;
    void readProjFromJSON() ;
    void writeProjToJSON()  ;

    void saveCallToFile()   ;
    void readCallFromFile() ;
    void readCallFromJSON() ;
    void writeCallToJSON()  ;

    void licenseControl()   ;

    QString getLastIndex (QString table);

private slots:


};

#endif // CONTROLLER_MAIN_H
