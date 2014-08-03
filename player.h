#ifndef PLAYER_H
#define PLAYER_H

#include "abstractsprite.h"
#include "common.h"
#include <QGraphicsItem>
#include <QPixmap>
#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QPointF>
#include <QKeyEvent>
#include <QTimer>
#include <QRect>
#include <QPainterPath>

#include <QtWidgets>

class Player : public AbstractSprite
{
    Q_OBJECT
public:
    Player(const QPoint& pos, int rightScene, const QPixmap& pix, QGraphicsItem* parent = nullptr);
    ~Player() = default;

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget* = nullptr) override;
    void setSpeed(int msec) override;
    QPainterPath shape() const override;
    void stopGame() override;

signals:
    void fire(const QPoint& point);

private:
    QPoint              m_posBoundingSprite;
    QPixmap             m_pixSprite;
    QRect               m_rectSprite;
    Common::MoveSprite  m_moveSprite;
    int                 m_widthBounding;
    int                 m_timerId = -1;

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void timerEvent(QTimerEvent* event) override;
    bool outputAbroad() override;
};

#endif // PLAYER_H
