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

    for(int i = 0; i < 9; i++)
        m_pixExplosion_.push_back(QPixmap(QString(":/explosion/resource/explosion/explosion_%1.png")
                                        .arg(QString::number(i))));
    this->connect(m_animExplosion, &QTimer::timeout, this, &Player::animExplosion);
}

QRectF Player::boundingRect() const
{
    if(m_flagExplosion)
        return QRectF(m_rectSprite.x(), m_posBoundingSprite.y() - 30,
                      m_pixExplosion_[m_flagExplosion].width(),
                      m_pixExplosion_[m_flagExplosion].height());
    else
        return QRectF(m_posBoundingSprite.x(), m_posBoundingSprite.y(),
                      m_widthBounding, m_pixSprite.height());
}

void Player::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
   //painter->drawRect(boundingRect());
    painter->drawPixmap(m_rectSprite, m_pixSprite);
    if(m_animExplosion->isActive())
        painter->drawPixmap(m_rectSprite.x(), m_posBoundingSprite.y() - 30
                        , m_pixExplosion_[m_frameIndex]);
}

void Player::setSpeed(int msec)
{
    if(msec > 0)
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

    if(event->key() == Qt::Key_Space && m_timerId != -1)
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

QPainterPath Player::shape() const
{
    QPainterPath path;
    path.addRect(m_rectSprite.x(), m_rectSprite.y(), m_rectSprite.width(), m_rectSprite.height());
    return path;
}

void Player::stopGame()
{
    if(m_timerId != -1)
    {
        this->killTimer(m_timerId);
        m_timerId = -1;
    }
}

void Player::animExplosion()
{
    m_frameIndex += 1;
    if(m_frameIndex == m_pixExplosion_.size())
        m_animExplosion->stop();
    this->update();
}

void Player::explosion()
{
    this->prepareGeometryChange();
    m_flagExplosion = true;
    m_animExplosion->start(100);
    QSound::play(":/sound/resource/sound/explosion.wav");
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

