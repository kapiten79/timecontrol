#include "class_send_tcp_request.h"

#include <iostream>
//#include <class_crypto.h>

using namespace std;

class_send_tcp_request::class_send_tcp_request(QObject *parent) : QObject(parent)
{
   // qDebug() << "Создан экземпляр класса class_send_tcp_request";

}

/* Соединение с сервером, инициация процесса отправки запроса */
void class_send_tcp_request::select()
{
    qDebug() << "Функция select запустилась (this)";
    ansver = "";
    connect(socket  , SIGNAL(connected())           ,this,      SLOT(connected())               );
    connect(socket  , SIGNAL(disconnected())        ,this,      SLOT(disconnected())            );
    connect(socket  , SIGNAL(bytesWritten(qint64))  ,this,      SLOT(bytesWritten(qint64))      );
    connect(socket  , SIGNAL(readyRead())           ,this,      SLOT(readyRead())               );

    qDebug() << "Запрос " << query;
    rq_qb = query.toUtf8() ;

    socket->connectToHost(servAddress, servPort) ;

    //qDebug() << socket->waitForConnected();
    if(socket->waitForConnected(10000))
    {
        qDebug() << "Ошибка: " << socket->errorString();
        ansver = "{\"info_type\":\"error\",\"error_text\":\"ERROR5: Нет связи с сервером\"}";
    }
}


/* Запись в сокет информации после соединения */
void class_send_tcp_request::connected()
{
    qDebug() << "Функция connected запустилась (this)";
    qDebug() << "connected..."          << rq_st.size()             ;
    socket->write(rq_qb/*+"##EnD##"*/) ;

    rq_st = ""              ;
    rq_qb = ""              ;
    if (!socket->waitForBytesWritten(10000))
    {
        qDebug() << "Ошибка! Данные не записаны!";
    }
}

/* Инициация ожидания ответа от сервера */
void class_send_tcp_request::bytesWritten(qint64 bytes)
{
    qDebug() << "Функция bytesWritten запустилась (this)";
    qDebug() << bytes << " bytes written...";
}

/* Приём ответа от сервера */
void class_send_tcp_request::readyRead()
{
    qDebug() << "Функция readyRead запустилась (this)";
    ansver.clear();
    ansver.append(socket->readAll());
    while(ansver.indexOf('}') == -1)
    {
        if (socket->waitForReadyRead(5000))  // ждём очередную порцию данных не дольше 5 секунд
        {
            qDebug() << "Дождались";
            ansver.append(socket->readAll());  // если дождались, то читаем
        }
        else
        {
            qDebug() << "Не дождались";
            break;  // если за 5 секунд данные не пришли, то прерываем цикл
        }
    }
    socket->disconnectFromHost();
    qDebug() << "ТЕКСТ ОТВЕТА " << QString::fromUtf8(ansver);
    //qDebug() << "ansver count " << ansver.count()   ;
    handlingAnsver();
}

/* Отключение от сервера после получения ответа */
void class_send_tcp_request::disconnected()
{
    //qDebug() << "Функция disconnected запустилась (this)";
    socket->disconnect();
    //qDebug() << "disconnected...";
    socket->close();

    disconnect(socket, 0,this,0);
    disconnect(socket, 0,this,0);
    disconnect(socket, 0,this,0);
    disconnect(socket, 0,this,0);
}

/* Преобразование ответа в массив JSON для дальнейшей обработки */
void class_send_tcp_request::handlingAnsver()
{
    //qDebug() << "Функция handlingAnsver запустилась (this)";
    QJsonParseError p_j_error;
    QJsonDocument d_ansver = QJsonDocument::fromJson(ansver,&p_j_error);
    QJsonObject o_ansver = d_ansver.object();
    qDebug() << "Ошибка парсинга json " << p_j_error.errorString();
    j_result = o_ansver["result"].toArray();
    //qDebug() << "Запрос выполнен";
    emit endReadRequest();
    disconnect(this, 0,this,0);
}


class_send_tcp_request::~class_send_tcp_request()
{
    delete socket   ;
}

/* Присвоение id авторизованного пользователя */
void class_send_tcp_request::setUserId(QString userId)
{
    qDebug() << "Функция setUserId(QString userId) запустилась";

    currUserId = userId;
    query = "{\"query\":\"SELECT name FROM root.users WHERE usersid = '"+currUserId+"'\",\"column_count\":\"1\"}";
    select();
    socket->waitForDisconnected();
    userName        = j_result[0].toArray()[0].toString();
}
