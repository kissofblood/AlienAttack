#ifndef COMMON_H
#define COMMON_H

#include <QSize>

namespace Common {

enum class Person { Player, Enemy };
enum class MoveSprite { TurnLeft, TurnRight, Stop };
static const QSize sizeScene(995, 615);
static const int speedAnimEnemy = 70;

}

#endif // COMMON_H
