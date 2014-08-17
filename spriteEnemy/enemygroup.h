#ifndef ENEMYGROUP_H
#define ENEMYGROUP_H

#include "shot.h"
#include "enemy.h"
#include "abstractsprite.h"
#include "player.h"
#include "helpenemy.h"
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
    void setAnimation(HelpEnemy* helpEnemy);

signals:
    void pathShot(Shot* shot);
    void killEnemy(int amount);
    void gameOver();

private slots:
    void shot(const QPoint& pos);
    void deleteShotItem(Shot* shotItem);
    void countDownEnemy(Common::MoveSprite moveSprite);
    void randomShotEnemy();
    void randomHelpEnemy();
    void insertHelpEnemy(QTimer* timer, QVector<QPair<Enemy*, Enemy*>>& randNewEnemy);

private:
    QPoint                      m_posBoundingSprite;
    QSize                       m_sizeBoundingSprite;
    Player                      *m_player           = nullptr;
    QGraphicsItemGroup          *m_group            = new QGraphicsItemGroup(this);
    QTimer                      *m_randShotEnemy    = new QTimer(this);
    QTimer                      *m_randHelpEnemy    = new QTimer(this);
    QVector<QVector<Enemy*>>    m_enemy_;
    QVector<Shot*>              m_shot_;
    QVector<QPixmap>            m_pixEnemy;
    QPoint                      m_posStartEnemy;
    QVector<std::function<void(Enemy*, QPoint, bool)>> m_animationHelp_;
    int m_row           = 4;
    int m_countYDown    = 0;
    int m_speedEnemy;



        QTimer                      *m_helpEnemy        = new QTimer(this);

    Enemy* m_enemy = nullptr;

    HelpEnemy* m_help = nullptr;

    bool outputAbroad() override;
};


#endif // ENEMYGROUP_H
