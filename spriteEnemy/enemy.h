#ifndef ENEMY_H
#define ENEMY_H

#include "abstractsprite.h"
#include "common.h"
#include <QPoint>
#include <QPixmap>
#include <QVector>
#include <QGraphicsItem>
#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QTimer>
#include <QPair>
#include <QPainterPath>
#include <QTimer>


#include <QtWidgets>

class Enemy : public AbstractSprite
{
    Q_OBJECT
    Q_PROPERTY(QPoint scenePosSprite READ scenePosSprite WRITE setScenePosSprite)
public:
    Enemy(const QPoint& pos, const QVector<QPixmap>& pix, QGraphicsItem* parent = nullptr);
    Enemy(const QPoint& pos, const QPair<int, int>& lefrAndRigh, const QVector<QPixmap>& pix, QGraphicsItem* parent = nullptr);
    ~Enemy() = default;
    Enemy& operator +=(Enemy* enemy);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget* = nullptr) override;
    void setSpeed(int msec) override;
    void stopGame() override;
    QPainterPath shape() const override;
    void attack();
    void downSprite();
    QPoint posSpiteTopLeft() const;
    QPoint posSpriteTopRight() const;
    QPoint scenePosSprite() const;
    void setScenePosSprite(const QPoint& pos);

signals:
    void fire(const QPoint& point);
    void turnChange(Common::MoveSprite moveSprite);

public slots:
    void setTurn(Common::MoveSprite moveSprite);

private slots:
    void animSprite();

private:
    QPoint              m_posBoundingSprite;
    QVector<QPixmap>    m_pixSprite_;
    QRect               m_rectSprite;
    QTimer              *m_animSprite       = new QTimer(this);
    Common::MoveSprite  m_moveSprite        = Common::MoveSprite::TurnRight;
    int                 m_frameIndex        = 0;
    int                 m_timerId           = -1;
    bool                m_flagForwardOrBack = true;
    int                 m_widthBounding;
    int                 m_yFire;
    bool                m_animSpriteUpdate   = false;
    int                 m_speedSprite        = -1;
    bool                m_stopScenePosSprite = false;

    void timerEvent(QTimerEvent* event) override;
    bool outputAbroad() override;
};

#endif // ENEMY_H
