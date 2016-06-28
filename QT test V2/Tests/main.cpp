#include "testbut.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestBut w;
    w.show();

    return a.exec();
}
