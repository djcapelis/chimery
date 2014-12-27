#include<QApplication>
#include<QPoint>

#include"chimerymainwin.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChimeryMainWindow w;
    w.show();
    QPoint pos = w.pos();
    pos.setY(pos.y() / 2);
    w.move(pos);

    return a.exec();
}
