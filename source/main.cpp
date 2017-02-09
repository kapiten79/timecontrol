#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.init_windowParams();

    return a.exec();
}
