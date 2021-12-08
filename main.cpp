#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;

    QObject::connect(&a,&QApplication::aboutToQuit,&w,&Widget::exit);

    w.setFixedSize(550,465);
    w.show();

    return a.exec();
}
