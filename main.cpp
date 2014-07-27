#include "alienattack.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AlienAttack w;
    w.show();

    return a.exec();
}
