#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //加载qss样式表
    QFile qssFile(":/qss/GLPsystem");
    qssFile.open(QFile::ReadOnly);
    qApp->setStyleSheet(qssFile.readAll());
    qssFile.close();
    Widget *w = new Widget();
    w->show();

    return a.exec();
}
