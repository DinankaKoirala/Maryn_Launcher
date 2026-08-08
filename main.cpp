#include "Mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    qDebug() << "Starting app...";
    QApplication a(argc, argv);
    qDebug() << "QApplication created";
    MainWindow w;
    qDebug() << "MainWindow created";
    w.show();
    qDebug() << "Window shown";
    return QApplication::exec();
}