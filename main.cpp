#include "alienattack.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AlienAttack w;
    //w.setFixedSize(1000, 620);
    w.show();

    return a.exec();
}
