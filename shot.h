#ifndef SHOT_H
#define SHOT_H

#include "abstractsprite.h"
#include <QGraphicsItem>
#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QPoint>
#include <QVector>
#include <QPixmap>
#include <QTimerEvent>

#include <QtWidgets>

class Shot : public AbstractSprite
{
    Q_OBJECT
public:
    explicit Shot(const QPoint& pos, const int height, const QVector<QPixmap>& vecPix, QGraphicsItem* parent = nullptr);
    ~Shot() = default;

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget* = nullptr) override;
    void setSpeed(int msec) override;

signals:
    void deleteShot();

private:
    QPoint                  m_posBoundingSprite;
    QVector<QPixmap>        m_pixSprite_;
    QRect                   m_rectSprite;
    int                     m_timerId = -1;
    int                     m_heightBounding;
    int                     m_indexAnimPix = 0;

    void timerEvent(QTimerEvent* event) override;
};

#endif // SHOT_H
