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

private slots:
    void shot(const QPoint& pos);
    void deleteShotItem();
    void countDownEnemy(Common::MoveSprite moveSprite);

private:
    QPoint                      m_posBoundingSprite;
    QSize                       m_sizeBoundingSprite;
    QGraphicsItemGroup          *m_group = new QGraphicsItemGroup(this);
    QVector<QVector<Enemy*>>    m_enemy_;
    QVector<Shot*>              m_shot_;
    int m_row       = 4;
    int m_column    = 7;
    int m_timerId   = -1;

    void timerEvent(QTimerEvent*) override;
    bool outputAbroad() final { return false; }
};


#endif // ENEMYGROUP_H
