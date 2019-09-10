#include <QApplication>
#include "mainwnd.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    QDir::setCurrent(qApp->applicationDirPath());

    mainwnd wnd;

    return a.exec();
}
