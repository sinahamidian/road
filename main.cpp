#include <QtGui/QApplication>
#include "mainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.CREATED = 0;
    w.show();
    return a.exec();
}
