#ifndef SHOT_H
#define SHOT_H

#include "abstractsprite.h"
#include "common.h"
#include <QGraphicsItem>
#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QPoint>
#include <QVector>
#include <QPixmap>
#include <QTimerEvent>
#include <QtMultimedia/QSound>
#include <QPainterPath>

#include <QtWidgets>

class Shot : public AbstractSprite
{
    Q_OBJECT
public:
    Shot(Common::Person person, const QPoint& pos, int height, const QVector<QPixmap>& vecPix, QGraphicsItem* parent = nullptr);
    ~Shot() = default;

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget* = nullptr) override;
    void setSpeed(int msec) override;
    QPainterPath shape() const override;
    void stopGame() override;

signals:
    void pathShot(Shot* shot);
    void deleteShot(Shot* shot);

private:
    QRect                   m_rectSprite;
    Common::Person          m_person;
    QVector<QPixmap>        m_pixSprite_;
    QPoint                  m_posBoundingSprite;
    int                     m_timerId = -1;
    int                     m_heightBounding;
    int                     m_indexAnimPix = 0;

    void timerEvent(QTimerEvent* event) override;
    bool outputAbroad() override;
};

#endif // SHOT_H
