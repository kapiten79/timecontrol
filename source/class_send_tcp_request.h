#ifndef CLASS_SEND_TCP_REQUEST_H
#define CLASS_SEND_TCP_REQUEST_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QAbstractSocket>
#include <QDebug>
#include <QHostAddress>
#include <sstream>
#include <QSettings>
#include <QFile>

using namespace std;

class class_send_tcp_request: public QObject
{
    Q_OBJECT
public:

    explicit class_send_tcp_request(QObject *parent = 0);
    ~class_send_tcp_request();

    void select()       ;

    /* Ссылка на объекты экземпляров классов, связанных с окнами QML */
    /** Подтверждённые переменные */
    QString servAddress = "127.0.0.1";
    int     servPort    = 7252;
    /** Идентификатор магазина */
    QString stationId = "1";

    /** Идентификатор прайс листа */
    QString priceId = "1";

    QVariantList p_params;

    vector<string>                  fields      ;
    QString                          query       ;
    QVector<QVector<QVariant> >      v_result      ;

    QString     rq_st               ;
    QByteArray  rq_qb           ;


    QObject     *parr_obj           ;
    QObject     *qmlObject          ;
    QString     slotFlag            ;


    QByteArray  ansver              ;
    QByteArray  file_byte_array     ;
    QString     ip_addr             ;

    QJsonArray j_result;

    QVariantList paramEndToEnd;

    QString fileName;

    QString currUserId;
    QString userName;

    QTcpSocket *socket = new QTcpSocket(this);


signals:
    void endReadRequest();

public slots:
     void connected()               ;
     void disconnected()            ;
     void bytesWritten(qint64 bytes);
     void readyRead()               ;

     void handlingAnsver()          ;
     void setUserId(QString userId);

private:

};

#endif // CLASS_SEND_TCP_REQUES_H
