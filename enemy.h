#ifndef ENEMY_H
#define ENEMY_H

#include "abstractsprite.h"
#include <QPoint>
#include <QPixmap>
#include <QVector>
#include <QGraphicsItem>
#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QTimer>


#include <QtWidgets>

class Enemy : public AbstractSprite
{
    Q_OBJECT

    enum class MoveSprite { TurnLeft, TurnRight };
public:
    Enemy(const QPoint& pos, int rightScene, const QVector<QPixmap>& pix, QGraphicsItem* parent = nullptr);
    ~Enemy() = default;

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget* = nullptr) override;
    void setSpeed(int msec) override;
    void startAnimSprite(int msec);

signals:
    void fire(const QPoint& point);

private slots:
    void animSprite();

private:
    QPoint              m_posBoundingSprite;
    QVector<QPixmap>    m_pixSprite_;
    QRect               m_rectSprite;
    QTimer              *m_animSprite       = new QTimer(this);
    MoveSprite          m_moveSprite        = MoveSprite::TurnLeft;
    int                 m_frameIndex        = 0;
    int                 m_timerId           = -1;
    bool                m_flagForwardOrBack = true;
    int                 m_widthBounding;

    void timerEvent(QTimerEvent* event) override;
    bool outputAbroad() override;
};

#endif // ENEMY_H
