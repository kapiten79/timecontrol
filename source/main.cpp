#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.init_windowParams();

//    #ifdef Q_OS_WIN32
//       QMessageBox::warning(0, QObject::tr("System"), QObject::tr("Windows"));
//    #endif

//    #ifdef Q_OS_LINUX
//            QMessageBox::warning(0, QObject::tr("System"), QObject::tr("Linux"));
//    #endif

    return a.exec();
}
