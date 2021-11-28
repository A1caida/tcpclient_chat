#include "widget.h"
#include <QApplication>
#include <QTcpSocket>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //login l;
    Widget w;

    QObject::connect(&a,&QApplication::aboutToQuit,&w,&Widget::exit);

    w.setFixedSize(680,500);
    w.show();

    return a.exec();
}
