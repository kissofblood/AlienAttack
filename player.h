#ifndef PLAYER_H
#define PLAYER_H

#include "abstractsprite.h"
#include <QGraphicsItem>
#include <QPixmap>
#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QPoint>
#include <QKeyEvent>
#include <QTimer>
#include <QRect>

#include <QtWidgets>

#include <QPropertyAnimation>

class Player : public AbstractSprite
{
    enum class MoveSprite { StartLeft, StartRight, Stop };
public:
    Player(const QPoint& pos, int rightScene, const QPixmap& pix, QGraphicsItem* parent = nullptr);
    ~Player() = default;

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget* = nullptr) override;
    void setSpeed(int speed) override;

private:
    QPoint          m_posBoundingSprite;
    QPixmap         m_pixSprite;
    QRect           m_rectSprite;
    MoveSprite      m_moveSprite;
    int             m_widthBounding;
    int             m_speedSprite   = 10;
    int             m_timerId       = -1;

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void timerEvent(QTimerEvent* event) override;
};

#endif // PLAYER_H
