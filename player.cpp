#include "player.h"

Player::Player(const QPoint& pos, int rightScene, const QPixmap& pix, QGraphicsItem* parent) : AbstractSprite(parent)
    , m_posBoundingSprite(pos)
    , m_pixSprite(pix)
    , m_rectSprite(pos.x(), pos.y(), pix.width(), pix.height())
{
    this->setFlag(QGraphicsItem::ItemIsFocusable);
    this->setFocus();

    m_widthBounding = rightScene - 5 - pos.x();
    m_moveSprite = MoveSprite::Stop;
    m_speedSprite = 10;
    this->startTimer(m_speedSprite);
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

void Player::setSpeed(int speed)
{
    if(speed != -1)
    {
        if(m_timerId != -1)
            this->killTimer(m_timerId);
        m_speedSprite = speed;
        m_timerId = -1;
    }
}

void Player::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_A || event->key() == Qt::Key_Left)
        m_moveSprite = MoveSprite::StartLeft;
    else if(event->key() == Qt::Key_D || event->key() == Qt::Key_Right)
        m_moveSprite = MoveSprite::StartRight;
}

void Player::keyReleaseEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_A || event->key() == Qt::Key_Left)
        m_moveSprite = MoveSprite::Stop;
    else if(event->key() == Qt::Key_D || event->key() == Qt::Key_Right)
        m_moveSprite = MoveSprite::Stop;
}

void Player::timerEvent(QTimerEvent*event)
{
    QRectF rectField = boundingRect();
    if(m_moveSprite == MoveSprite::StartLeft)
    {
        bool flagLeft = true;
        if(rectField.left() >= m_rectSprite.left())
        {
            m_rectSprite.moveLeft(rectField.left());
            flagLeft = false;
        }
        if(flagLeft)
            m_rectSprite.moveLeft(m_rectSprite.left() - 2);
    }
    else if(m_moveSprite == MoveSprite::StartRight)
    {
        bool flagRight = true;
        if(rectField.right() <= m_rectSprite.right())
        {
            m_rectSprite.moveRight(rectField.right());
            flagRight = false;
        }
        if(flagRight)
            m_rectSprite.moveRight(m_rectSprite.right() + 2);
    }
    m_timerId = event->timerId();
    this->update(boundingRect());
}
