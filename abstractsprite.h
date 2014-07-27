#ifndef ABSTRACTSPRITE_H
#define ABSTRACTSPRITE_H

#include <QGraphicsObject>
#include <QGraphicsItem>

class AbstractSprite : public QGraphicsObject
{
public:
    AbstractSprite(QGraphicsItem* parent = nullptr);
    ~AbstractSprite() = default;

    virtual void setSpeed(int msec) = 0;
};

#endif // ABSTRACTSPRITE_H
