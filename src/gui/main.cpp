#include "freelssmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FreeLSSMainWindow w;
    w.show();

    return a.exec();
}
