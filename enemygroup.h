#ifndef ENEMYGROUP_H
#define ENEMYGROUP_H

#include "shot.h"
#include "enemy.h"
#include "abstractsprite.h"
#include "player.h"
#include <QGraphicsItemGroup>
#include <QGraphicsItem>
#include <QVector>
#include <QPoint>
#include <algorithm>
#include <functional>
#include <QTimer>
#include <QSize>

#include <QtWidgets>

class EnemyGroup : public AbstractSprite
{
    Q_OBJECT
public:
    EnemyGroup(const QPoint& pos, int rightScene, Player* player, QGraphicsItem* parent = nullptr);
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
    void gameOver();

private slots:
    void shot(const QPoint& pos);
    void deleteShotItem(Shot* shotItem);
    void countDownEnemy(Common::MoveSprite moveSprite);
    void randomShotEnemy();

private:
    QPoint                      m_posBoundingSprite;
    QSize                       m_sizeBoundingSprite;
    Player                      *m_player           = nullptr;
    QGraphicsItemGroup          *m_group            = new QGraphicsItemGroup(this);
    QTimer                      *m_randomShotEnemy  = new QTimer(this);
    QVector<QVector<Enemy*>>    m_enemy_;
    QVector<Shot*>              m_shot_;

    bool outputAbroad() override;
};


#endif // ENEMYGROUP_H
