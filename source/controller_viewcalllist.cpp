#include "controller_viewcalllist.h"

controller_viewCallList::controller_viewCallList()
{

}

void controller_viewCallList::init_window()
{
    qDebug() << "Функция init_window запустилась " << workDir;
    readCallFromFile();
    qDebug() << "Прочлось";
    cList.callDate          .clear();
    cList.callTime          .clear();
    cList.callIndex         .clear();
    cList.callProject       .clear();
    cList.callTopics        .clear();
    cList.callDescription   .clear();

    qDebug() << "Количество записей, полученных из файла " << j_dataArray.count();
    for (int i=0; i<j_dataArray.count(); i++)
    {
        qDebug() << "Очередной заголовок " << j_dataArray[i].toArray()[4].toString();
        qDebug() << "Очередное описание " << j_dataArray[i].toArray()[5].toString();
        cList.callDate        << j_dataArray[i].toArray()[0].toString();
        cList.callTime        << j_dataArray[i].toArray()[1].toString();
        cList.callIndex       << j_dataArray[i].toArray()[2].toString();
        cList.callProject     << j_dataArray[i].toArray()[3].toString();
        cList.callTopics      << j_dataArray[i].toArray()[4].toString();
        cList.callDescription << j_dataArray[i].toArray()[5].toString();
    }
    connect (this, SIGNAL(s_loadCallList()), model, SLOT(loadCallList()));
    emit s_loadCallList();
    disconnect (this, SIGNAL(s_loadCallList()), model, SLOT(loadCallList()));
}
