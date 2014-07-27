#include "battlefield.h"

Battlefield::Battlefield(const QRectF& rect, QObject* parent) : QGraphicsScene(rect, parent)
    , m_player(new Player(QPoint(300, 550), rect.right(), QPixmap(":player/resource/player.png")))
{
    this->addPixmap(QPixmap(":/titlescreen/resource/bgScreen.png"));
    this->addItem(m_player);

    this->connect(m_player, &Player::fire, this, &Battlefield::shot);
}

void Battlefield::shot(const QPoint& pos)
{
    const int countPix = 2;
    QVector<QPixmap> vecPix;
    for(int i = 0; i < countPix; i++)
        vecPix.push_back(QPixmap(QString(":shot/playershot/resource/shot/pshot_%1.png")
                                         .arg(QString::number(i))));

    m_shot_.push_back(new Shot(pos, 550, vecPix));
    this->addItem(m_shot_.back());
    this->connect(m_shot_.back(), &Shot::deleteShot, this, &Battlefield::deleteShotItem);
}

void Battlefield::deleteShotItem()
{
    Shot* shotItem = qobject_cast<Shot*>(this->sender());
    if(shotItem == nullptr)
        return;

    m_shot_.erase(std::remove_if(m_shot_.begin(), m_shot_.end(), [shotItem](Shot* shot)
    { return shot == shotItem; }), m_shot_.end());
    this->removeItem(shotItem);
    delete shotItem;
}
