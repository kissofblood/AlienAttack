#ifndef ENEMYGROUP_H
#define ENEMYGROUP_H

#include "shot.h"
#include "enemy.h"
#include "abstractsprite.h"
#include "common.h"
#include <QGraphicsItemGroup>
#include <QGraphicsItem>
#include <QVector>
#include <QPoint>
#include <algorithm>
#include <functional>
#include <QTimer>
#include <QSize>
#include <QPointer>
#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QObject>
#include <QPixmap>
#include <QPair>
#include <QTime>
#include <QString>
#include <QPropertyAnimation>
#include <QAbstractAnimation>
#include <QEasingCurve>

class EnemyGroup : public AbstractSprite
{
    Q_OBJECT
public:
    EnemyGroup(const QPoint& pos, int rightScene, QGraphicsItem* parent = nullptr);
    ~EnemyGroup() = default;

    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* = nullptr) override { }
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
    void randomHelpEnemy();
    void insertHelpEnemy();

private:
    class HelpEnemy : public QGraphicsPixmapItem
    {
    public:
        HelpEnemy(const QVector<QPixmap>& pix, QGraphicsItem* parent = nullptr);
        ~HelpEnemy() override;

        void animationHelp(Enemy* enemy, const QPoint& posEnd);

    private:
        QVector<QPixmap>    m_pixEnemy_;
        QTimer              *m_animHelp = new QTimer;
    };

    QPoint                   m_posBoundingSprite;
    QSize                    m_sizeBoundingSprite;
    QGraphicsItemGroup       *m_group            = new QGraphicsItemGroup(this);
    QTimer                   *m_randShotEnemy    = new QTimer(this);
    QTimer                   *m_randHelpEnemy    = new QTimer(this);
    QTimer                   *m_insertHelpEnemy  = new QTimer(this);
    HelpEnemy                *m_helpEnemy        = nullptr;
    QVector<QVector<Enemy*>> m_enemy_;
    QVector<Shot*>           m_shot_;
    QVector<QPixmap>         m_pixEnemy;
    QPoint                   m_posStartEnemy = { 20, 200 };
    QVector<QPair<Enemy*, QPointer<Enemy>>> m_randNewEnemy_;
    int m_row           = 4;
    int m_speedEnemy;

    bool outputAbroad() override;
};

#endif // ENEMYGROUP_H
