#include "analizator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Analizator w;
    w.show();

    return a.exec();
}
