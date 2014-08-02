#ifndef COMMON_H
#define COMMON_H

#include <QSize>

namespace Common {

enum class Person { Player, Enemy };
enum class MoveSprite { TurnLeft, TurnRight, Stop };
static QSize sizeScene(995, 615);

}

#endif // COMMON_H
