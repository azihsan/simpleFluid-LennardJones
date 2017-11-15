#include "mdsimulator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mdSimulator w;
    w.show();

    return a.exec();
}
