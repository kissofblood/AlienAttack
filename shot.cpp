#include "shot.h"

Shot::Shot(Common::Person person, const QPoint& pos, int height, const QVector<QPixmap>& vecPix, QGraphicsItem* parent) : AbstractSprite(parent)
    , m_rectSprite(QRect(pos.x(), pos.y(), vecPix.front().width(), vecPix.front().width()))
    , m_person(person)
{
    if(person == Common::Person::Player)
    {
        m_posBoundingSprite = QPoint(pos.x(), pos.y() - vecPix.front().height() + 4);
        QSound::play(":sound/resource/sound/playerShot.wav");
    }
    else if(person == Common::Person::Enemy)
    {
        m_posBoundingSprite = pos;
        QSound::play(":sound/resource/sound/enemyShot.wav");
    }
    m_pixSprite_ = vecPix;
    m_heightBounding = height;
    setSpeed(5);
}

QRectF Shot::boundingRect() const
{
    if(m_person == Common::Person::Player)
        return QRectF(m_posBoundingSprite.x(), 0, m_pixSprite_.front().width(), m_heightBounding);
    else if(m_person == Common::Person::Enemy)
        return QRectF(m_posBoundingSprite.x(), m_posBoundingSprite.y(), m_pixSprite_.front().width(), m_heightBounding);
    return QRectF();
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
    if(outputAbroad())
    {
        m_timerId = -1;
        this->killTimer(event->timerId());
            emit deleteShot();
        return;
    }
    m_timerId = event->timerId();
    this->update(boundingRect());
}

bool Shot::outputAbroad()
{
    if(m_person == Common::Person::Player)
    {
        if(m_rectSprite.top() <= 0)
            return true;
        m_rectSprite.moveTo(m_rectSprite.left(), m_rectSprite.top() - 2);
        return false;
    }
    else if(m_person == Common::Person::Enemy)
    {
        if(m_rectSprite.top() >= boundingRect().bottom())
            return true;
        m_rectSprite.moveTo(m_rectSprite.left(), m_rectSprite.top() + 2);
        return false;
    }
    return false;
}
