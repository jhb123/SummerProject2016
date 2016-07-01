
#include <QApplication>
#include "touchscreen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TouchScreen w;
    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    w.show();

    return a.exec();
}
