#include "mainwindow.h"
#include <QApplication>
#include "file.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle("fusion");

    MainWindow w;
    w.show();

    return a.exec();
}
