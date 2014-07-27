#include "shot.h"

Shot::Shot(const QPoint& pos, const int height, const QVector<QPixmap>& vecPix, QGraphicsItem* parent) : AbstractSprite(parent)
    , m_posBoundingSprite(QPoint(pos.x(), pos.y() - vecPix.front().height() + 4))
    , m_rectSprite(QRect(pos.x(), pos.y(), vecPix.front().width(), vecPix.front().width()))
{
    m_pixSprite_ = vecPix;
    m_heightBounding = height;
    this->startTimer(5);
}

QRectF Shot::boundingRect() const
{
    return QRectF(m_posBoundingSprite.x(), 0, m_pixSprite_.front().width(), m_heightBounding);
}

void Shot::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    //painter->drawRect(boundingRect());
    painter->drawPixmap(m_rectSprite, m_pixSprite_[m_indexAnimPix == m_pixSprite_.size()
                                                    ? m_indexAnimPix - 1 : m_indexAnimPix]);
}

void Shot::setSpeed(int msec)
{
    if(msec != -1)
    {
        if(m_timerId != -1)
            this->killTimer(m_timerId);
        m_timerId = -1;
        m_indexAnimPix = 0;
        this->startTimer(msec);
    }
}

void Shot::timerEvent(QTimerEvent* event)
{
    if(m_indexAnimPix < m_pixSprite_.size())
        m_indexAnimPix += 1;

    if(m_rectSprite.top() <= 0)
    {
        this->killTimer(event->timerId());
        m_timerId = -1;
        emit deleteShot();
        return;
    }
    m_rectSprite.moveTo(m_rectSprite.left(), m_rectSprite.top() - 2);
    m_timerId = event->timerId();
    this->update(boundingRect());
}
