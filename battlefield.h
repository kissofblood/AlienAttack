#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "player.h"
#include "enemygroup.h"
#include "shot.h"
#include "common.h"
#include <QObject>
#include <QGraphicsScene>
#include <QPixmap>
#include <QPoint>
#include <QRectF>
#include <QVector>
#include <algorithm>
#include <functional>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QFont>
#include <QColor>
#include <QGraphicsBlurEffect>
#include <QStateMachine>
#include <QState>
#include <QPropertyAnimation>
#include <QString>

class Battlefield : public QGraphicsScene
{
    Q_OBJECT
public:
    Battlefield(const QRectF& rect, QObject* parent = nullptr);
    ~Battlefield() = default;

signals:
    void transitionState1();
    void transitionState2();
    void gameOver();

private slots:
    void shot(const QPoint& pos);
    void deleteShotItem(Shot* shotItem);
    void reduceLife();
    void setScope();
    void collidingPlayer(Shot* shot);
    void collidingEnemy(Shot* shot);

private:
    Player              *m_player       = nullptr;
    EnemyGroup          *m_enemyGroup   = nullptr;
    QGraphicsTextItem   *m_itemTxtLive  = nullptr;
    QGraphicsPixmapItem *m_itemPixLive  = nullptr;
    QGraphicsTextItem   *m_itemTxtScope = nullptr;
    QStateMachine       *m_stateMachine = new QStateMachine(this);
    QVector<Shot*>      m_shot_;
};

#endif // BATTLEFIELD_H
