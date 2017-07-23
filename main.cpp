#include "mainwindow.h"
#include <QApplication>
/**
 * mainw.cpp
 * @author Valery Toda
 **/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
