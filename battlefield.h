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
#include <QTimer>

#include <QtWidgets>

class Battlefield : public QGraphicsScene
{
    Q_OBJECT
public:
    Battlefield(const QRectF& rect, QObject* parent = nullptr);
    ~Battlefield() = default;

    void setSpeedShotEnemy(int msec);
    void setSpeedEnemy(int msec);
    void activateTime();

signals:
    void transitionState1();
    void transitionState2();
    void gameOver();
    void killEnemy(int amount);
    void finishGame();

private slots:
    void shot(const QPoint& pos);
    void deleteShotItem(Shot* shotItem);
    void reduceLife();
    void setScope();
    void collidingPlayer(Shot* shot);
    void collidingEnemy(Shot* shot);
    void countdown();

private:
    Player              *m_player       = nullptr;
    EnemyGroup          *m_enemyGroup   = nullptr;
    QGraphicsTextItem   *m_itemTxtLive  = nullptr;
    QGraphicsPixmapItem *m_itemPixLive  = nullptr;
    QGraphicsTextItem   *m_itemTxtTimer = nullptr;
    QStateMachine       *m_stateMachine = new QStateMachine(this);
    QTimer              *m_timer        = new QTimer(this);
    QVector<Shot*>      m_shot_;
};

#endif // BATTLEFIELD_H
