#include "player.h"

Player::Player(const QPoint& pos, int rightScene, const QPixmap& pix, QGraphicsItem* parent) : AbstractSprite(parent)
    , m_posBoundingSprite(pos)
    , m_pixSprite(pix)
    , m_rectSprite(pos.x(), pos.y(), pix.width(), pix.height())
{
    this->setFlag(QGraphicsItem::ItemIsFocusable);
    this->setFocus();

    m_widthBounding = rightScene - 5 - pos.x();
    m_moveSprite = Common::MoveSprite::Stop;
    setSpeed(10);
}

QRectF Player::boundingRect() const
{
    return QRectF(m_posBoundingSprite.x(), m_posBoundingSprite.y(), m_widthBounding, m_pixSprite.height());
}

void Player::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
//    painter->drawRect(boundingRect());
    painter->drawPixmap(m_rectSprite, m_pixSprite);
}

void Player::setSpeed(int msec)
{
    if(msec != -1)
    {
        if(m_timerId != -1)
            this->killTimer(m_timerId);
        m_timerId = -1;
        this->startTimer(msec);
    }
}

void Player::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_A || event->key() == Qt::Key_Left)
        m_moveSprite = Common::MoveSprite::TurnLeft;
    else if(event->key() == Qt::Key_D || event->key() == Qt::Key_Right)
        m_moveSprite = Common::MoveSprite::TurnRight;

    if(event->key() == Qt::Key_Space)
        emit fire(QPoint(m_rectSprite.left() + m_rectSprite.width() / 2 - 3, m_rectSprite.top()));
}

void Player::keyReleaseEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_A || event->key() == Qt::Key_Left)
        m_moveSprite = Common::MoveSprite::Stop;
    else if(event->key() == Qt::Key_D || event->key() == Qt::Key_Right)
        m_moveSprite = Common::MoveSprite::Stop;
}

void Player::timerEvent(QTimerEvent*event)
{
    outputAbroad();
    m_timerId = event->timerId();
    this->update(boundingRect());
}

bool Player::outputAbroad()
{
    QRectF rectField = boundingRect();
    if(m_moveSprite == Common::MoveSprite::TurnLeft)
    {
        if(rectField.left() >= m_rectSprite.left())
        {
            m_rectSprite.moveLeft(rectField.left());
            return true;
        }
        m_rectSprite.moveLeft(m_rectSprite.left() - 2);
        return false;
    }
    else if(m_moveSprite == Common::MoveSprite::TurnRight)
    {
        if(rectField.right() <= m_rectSprite.right())
        {
            m_rectSprite.moveRight(rectField.right());
            return true;
        }
        m_rectSprite.moveRight(m_rectSprite.right() + 2);
        return false;
    }
    return false;
}

