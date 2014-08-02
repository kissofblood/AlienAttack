#include "enemy.h"

Enemy::Enemy(const QPoint& pos, const QPair<int, int>& lefrAndRigh, const QVector<QPixmap>& pix, QGraphicsItem* parent) : AbstractSprite(parent)
    , m_posBoundingSprite(lefrAndRigh.first, pos.y())
    , m_pixSprite_(pix)
    , m_rectSprite(pos.x(), pos.y(), pix.front().width(), pix.front().height())
{
    m_widthBounding = lefrAndRigh.second - 5 - lefrAndRigh.first;
    m_yFire = m_rectSprite.top() + m_pixSprite_[m_frameIndex].height() - 15;

    startAnimSprite(70);
    setSpeed(18);

    this->connect(m_animSprite, &QTimer::timeout, this, &Enemy::animSprite);
}

QRectF Enemy::boundingRect() const
{
    return QRectF(m_posBoundingSprite.x(), m_posBoundingSprite.y(), m_widthBounding, m_pixSprite_[m_frameIndex].height());
}

void Enemy::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    //painter->drawRect(boundingRect());
    painter->drawPixmap(m_rectSprite, m_pixSprite_[m_frameIndex]);
}

void Enemy::setSpeed(int msec)
{
    if(msec != -1)
    {
        if(m_timerId != -1)
            this->killTimer(m_timerId);
        m_timerId = -1;
        this->startTimer(msec);
    }
}

void Enemy::startAnimSprite(int msec)
{
    m_animSprite->start(msec);
}

void Enemy::attack()
{
    emit fire(QPoint(m_rectSprite.left() + m_rectSprite.width() / 2 - 3, m_yFire));
}

void Enemy::setPosY(qreal y)
{
    this->setY(y - 20);
    m_yFire += this->pos().y() - m_yFire;
    m_yFire += m_rectSprite.top() + m_pixSprite_[m_frameIndex].height() - 15;
}

void Enemy::setTurn(Common::MoveSprite moveSprite)
{
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
            return false;
        }
        m_rectSprite.moveLeft(m_rectSprite.left() - 2);
        return true;
    }
    else if(m_moveSprite == Common::MoveSprite::TurnRight)
    {
        if(rectField.right() <= m_rectSprite.right())
        {
            emit turnChange(Common::MoveSprite::TurnLeft);
            m_rectSprite.moveRight(rectField.right());
            m_moveSprite = Common::MoveSprite::TurnLeft;
            return false;
        }
        m_rectSprite.moveRight(m_rectSprite.right() + 2);
        return true;
    }
    return false;
}
