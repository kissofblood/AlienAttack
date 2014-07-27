#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "player.h"
#include <QObject>
#include <QGraphicsScene>
#include <QPixmap>
#include <QPoint>
#include <QRectF>

class Battlefield : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Battlefield(const QRectF& rect, QObject* parent = nullptr);
    ~Battlefield() = default;

private:
    Player      *m_player = nullptr;
};

#endif // BATTLEFIELD_H
