#ifndef ENEMYGROUP_H
#define ENEMYGROUP_H

#include "shot.h"
#include "enemy.h"
#include "abstractsprite.h"
#include <QGraphicsItemGroup>
#include <QGraphicsItem>
#include <QVector>
#include <QPoint>
#include <algorithm>
#include <functional>
#include <QTimerEvent>
#include <QSize>

#include <QtWidgets>

class EnemyGroup : public AbstractSprite
{
    Q_OBJECT
public:
    EnemyGroup(const QPoint& pos, int rightScene, QGraphicsItem* parent = nullptr);
    ~EnemyGroup() = default;

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget* = nullptr) override;
    void setSpeed(int msec) override;
    void setSpeedEnemy(int msec);
    void stopGame() override;
    void removeShotItem(Shot* shotItem);
    bool collidingEnemy(Shot* shot);

signals:
    void pathShot(Shot* shot);
    void killEnemy(int amount);

private slots:
    void shot(const QPoint& pos);
    void deleteShotItem(Shot* shotItem);
    void countDownEnemy(Common::MoveSprite moveSprite);

private:
    QPoint                      m_posBoundingSprite;
    QSize                       m_sizeBoundingSprite;
    QGraphicsItemGroup          *m_group = new QGraphicsItemGroup(this);
    QVector<QVector<Enemy*>>    m_enemy_;
    QVector<Shot*>              m_shot_;
    int                         m_timerId = -1;

    void timerEvent(QTimerEvent* event) override;
    bool outputAbroad() final { return false; }
};


#endif // ENEMYGROUP_H
