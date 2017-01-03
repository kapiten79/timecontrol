#ifndef CONTROLLER_EXPORTEXCEL_H
#define CONTROLLER_EXPORTEXCEL_H

#include <QObject>
    #ifdef Q_OS_WIN32
        #include <ActiveQt/qaxobject.h>
        #include <ActiveQt/qaxbase.h>
        #include <ActiveQt/qaxwidget.h>
    #endif

//    #ifdef Q_OS_LINUX
//            QMessageBox::warning(0, QObject::tr("System"), QObject::tr("Linux"));
//    #endif

#include <QFileDialog>
#include <QVector>

#include <QDate>
#include <QTime>

#include <QDebug>

class controller_exportExcel : public QObject
{
    Q_OBJECT
public:
    explicit controller_exportExcel(QObject *parent = 0);
    void process_export();

    QVariantList topics;
    QVariantList widthList;
    QVariantList total;
    QVector<QVector<QString>> qv;

    QString reportName;
    QString reportDate = QDate::currentDate().toString("dd/MM/yyyy");
    QString reportTime = QTime::currentTime().toString("hh:mm:ss");
    QString *workDir;

signals:

public slots:
};

#endif // CONTROLLER_EXPORTEXCEL_H
