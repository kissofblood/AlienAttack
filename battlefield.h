#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "player.h"
#include "shot.h"
#include <QObject>
#include <QGraphicsScene>
#include <QPixmap>
#include <QPoint>
#include <QRectF>
#include <QVector>
#include <algorithm>

class Battlefield : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Battlefield(const QRectF& rect, QObject* parent = nullptr);
    ~Battlefield() = default;

private:
    void shot(const QPoint& pos);
    void deleteShotItem();

private:
    Player          *m_player   = nullptr;
    QVector<Shot*>  m_shot_;
};

#endif // BATTLEFIELD_H
