#include "enemy.h"

Enemy::Enemy(const QPoint& pos, const QVector<QPixmap>& pix, QGraphicsItem* parent) : AbstractSprite(parent)
    , m_posBoundingSprite(pos)
    , m_pixSprite_(pix)
    , m_rectSprite(pos.x(), pos.y(), pix.front().width(), pix.front().height())
{
    m_widthBounding = pix.front().width();
    m_yFire = m_rectSprite.top() + m_pixSprite_[m_frameIndex].height() - 15;
    m_animSpriteUpdate = true;

    m_animSprite->start(Common::speedAnimEnemy);
    this->connect(m_animSprite, &QTimer::timeout, this, &Enemy::animSprite);
}

Enemy::Enemy(const QPoint& pos, const QPair<int, int>& lefrAndRigh, const QVector<QPixmap>& pix, QGraphicsItem* parent) : AbstractSprite(parent)
    , m_posBoundingSprite(lefrAndRigh.first, pos.y())
    , m_pixSprite_(pix)
    , m_rectSprite(pos.x(), pos.y(), pix.front().width(), pix.front().height())
{
    m_widthBounding = lefrAndRigh.second - 5 - lefrAndRigh.first;
    m_yFire = m_rectSprite.top() + m_pixSprite_[m_frameIndex].height() - 15;

    m_animSprite->start(Common::speedAnimEnemy);
    this->connect(m_animSprite, &QTimer::timeout, this, &Enemy::animSprite);
}

Enemy& Enemy::operator +=(Enemy* enemy)
{
    if(enemy == this)
        return *this;
    this->prepareGeometryChange();
    m_posBoundingSprite  = enemy->m_posBoundingSprite;
    m_moveSprite         = enemy->m_moveSprite;
    m_widthBounding      = enemy->m_widthBounding;
    m_yFire              = enemy->m_yFire;
    m_animSpriteUpdate   = false;
    m_stopScenePosSprite = true;
    m_rectSprite.moveLeft(enemy->m_rectSprite.right() + 5);
    setSpeed(enemy->m_speedSprite);
    return *this;
}

QRectF Enemy::boundingRect() const
{
    return QRectF(m_posBoundingSprite.x(), m_posBoundingSprite.y(), m_widthBounding, m_pixSprite_[m_frameIndex].height());
}

void Enemy::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->drawPixmap(m_rectSprite, m_pixSprite_[m_frameIndex]);
}

void Enemy::setSpeed(int msec)
{
    if(msec > 0)
    {
        if(m_timerId != -1)
            this->killTimer(m_timerId);
        m_timerId = -1;
        m_speedSprite = msec;
        this->startTimer(msec);
    }
}

void Enemy::stopGame()
{
    if(m_timerId != -1)
    {
        this->killTimer(m_timerId);
        m_animSprite->stop();
        m_timerId = -1;
    }
}

QPainterPath Enemy::shape() const
{
    QPainterPath path;
    path.addRect(m_rectSprite.x(), m_rectSprite.y(), m_rectSprite.width(), m_rectSprite.height());
    return path;
}

void Enemy::attack()
{
    emit fire(QPoint(m_rectSprite.left() + m_rectSprite.width() / 2 - 3, m_yFire));
}

void Enemy::downSprite()
{
    this->prepareGeometryChange();
    m_posBoundingSprite.ry() += 20;
    m_rectSprite.moveTop(m_posBoundingSprite.y());
    m_yFire += m_posBoundingSprite.y() - m_yFire;
    m_yFire += m_rectSprite.top() + m_pixSprite_[m_frameIndex].height() - 15;
}

QPoint Enemy::posSpiteTopLeft() const
{
    if(this->pos().y() == 20)
        return QPoint(m_rectSprite.left(), m_posBoundingSprite.y());
    else
        return QPoint(m_rectSprite.left(), m_posBoundingSprite.y() + this->pos().y());
}

QPoint Enemy::posSpriteTopRight() const
{
    if(this->pos().y() == 20)
        return QPoint(m_rectSprite.right(), m_posBoundingSprite.y());
    else
        return QPoint(m_rectSprite.right(), m_posBoundingSprite.y() + this->pos().y());
}

QPoint Enemy::scenePosSprite() const
{
    return m_posBoundingSprite;
}

void Enemy::setScenePosSprite(const QPoint& pos)
{
    if(!m_stopScenePosSprite)
    {
        this->prepareGeometryChange();
        m_posBoundingSprite = pos;
        m_rectSprite.moveTopLeft(pos);
    }
}

void Enemy::setTurn(Common::MoveSprite moveSprite)
{
    if(!m_stopScenePosSprite)
        m_rectSprite.moveLeft(m_rectSprite.left() - 2);
    m_moveSprite = moveSprite;
}

void Enemy::animSprite()
{
    if(m_frameIndex < m_pixSprite_.size() && m_flagForwardOrBack)
    {
        m_frameIndex += 1;
        if(m_frameIndex == m_pixSprite_.size() - 1)
            m_flagForwardOrBack = false;
    }
    else
    {
        m_frameIndex -= 1;
        if(m_frameIndex == 0)
            m_flagForwardOrBack = true;
    }

    if(m_animSpriteUpdate)
        this->update(boundingRect());
}

void Enemy::timerEvent(QTimerEvent* event)
{
    outputAbroad();
    m_timerId = event->timerId();
    this->update(boundingRect());
}

bool Enemy::outputAbroad()
{
    QRectF rectField = boundingRect();
    if(m_moveSprite == Common::MoveSprite::TurnLeft)
    {
        if(rectField.left() >= m_rectSprite.left())
        {
            emit turnChange(Common::MoveSprite::TurnRight);
            m_rectSprite.moveLeft(rectField.left());
            m_moveSprite = Common::MoveSprite::TurnRight;
            return true;
        }
        m_rectSprite.moveLeft(m_rectSprite.left() - 2);
        return false;
    }
    else if(m_moveSprite == Common::MoveSprite::TurnRight)
    {
        if(rectField.right() <= m_rectSprite.right())
        {
            emit turnChange(Common::MoveSprite::TurnLeft);
            m_rectSprite.moveRight(rectField.right());
            m_moveSprite = Common::MoveSprite::TurnLeft;
            return true;
        }
        m_rectSprite.moveRight(m_rectSprite.right() + 2);
        return false;
    }
    return false;
}
