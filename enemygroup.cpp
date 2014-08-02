#include "enemygroup.h"

EnemyGroup::EnemyGroup(const QPoint& pos, int rightScene, QGraphicsItem* parent) : AbstractSprite(parent)
    , m_posBoundingSprite(pos)
    , m_sizeBoundingSprite(rightScene - 5 - pos.x(), 250)
{
    qsrand(QTime::currentTime().second());

    const int countPix = 10;
    QVector<QPixmap> vecPix;
    for(int i = 0; i < countPix; i++)
        vecPix.push_back(QPixmap(QString(":enemy/resource/enemy/enemy1_%1").arg(QString::number(i))));
     for(int i = 0, y = 0; i < m_row; i++, y += vecPix.front().height() + 10)
     {
         QVector<Enemy*> vecEnemy;
         for(int j = 0, x = 0; j < m_column; j++, x += vecPix.front().width() + 5)
         {
             vecEnemy.push_back(new Enemy(QPoint(pos.x() + x, pos.y() + y), qMakePair(pos.x(), rightScene), vecPix));
             vecEnemy.back()->setSpeed(120);
             m_group->addToGroup(vecEnemy.back());
         }
         m_enemy_.push_back(vecEnemy);
     }

     std::for_each(m_enemy_.begin(), m_enemy_.end(), [this](const QVector<Enemy*>& vecEnemy)
     {
         foreach(Enemy* enemy1, vecEnemy)
         {
             foreach(Enemy* enemy2, vecEnemy)
                 if(enemy1 != enemy2)
                     this->connect(enemy1, &Enemy::turnChange, enemy2, &Enemy::setTurn);
             this->connect(enemy1, &Enemy::fire, this, &EnemyGroup::shot);
             this->connect(enemy1, &Enemy::turnChange, this, &EnemyGroup::countDownEnemy);
         }
     });

     setSpeed(1000);
}

QRectF EnemyGroup::boundingRect() const
{
    return QRectF(m_posBoundingSprite.x(), m_posBoundingSprite.y(), m_sizeBoundingSprite.width(), m_sizeBoundingSprite.height());
}

void EnemyGroup::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    //painter->drawRect(boundingRect());
}

void EnemyGroup::setSpeed(int msec)
{
    if(msec != -1)
    {
        if(m_timerId != -1)
            this->killTimer(m_timerId);
        m_timerId = -1;
        this->startTimer(msec);
    }
}

void EnemyGroup::shot(const QPoint& pos)
{
    const int countPix = 2;
    QVector<QPixmap> vecPix;
    for(int i = 0; i < countPix; i++)
        vecPix.push_back(QPixmap(QString(":shot/enemy/resource/shot/eshot_%1.png").arg(QString::number(i))));

    m_shot_.push_back(new Shot(Common::Person::Enemy, pos, Common::sizeScene.height() - pos.y(), vecPix));
    m_group->addToGroup(m_shot_.back());
    this->connect(m_shot_.back(), &Shot::deleteShot, this, &EnemyGroup::deleteShotItem);
}

void EnemyGroup::deleteShotItem()
{
    Shot* shotItem = qobject_cast<Shot*>(this->sender());
    if(shotItem == nullptr)
        return;

    m_shot_.erase(std::remove(m_shot_.begin(), m_shot_.end(), shotItem), m_shot_.end());
    m_group->removeFromGroup(shotItem);
    delete shotItem;
}

void EnemyGroup::countDownEnemy(Common::MoveSprite moveSprite)
{
    static int count = 0;
    static int countY = 0;
    if(moveSprite == Common::MoveSprite::TurnRight)
        count += 1;

    if(count == m_enemy_.size())
    {
        countY += 1;
        count = 0;
    }

    if(countY == 4)
    {
        m_posBoundingSprite.ry() += 20;
        std::for_each(m_enemy_.begin(), m_enemy_.end(), [this](const QVector<Enemy*>& vecEnemy)
        {
            foreach(Enemy* enemy, vecEnemy)
                enemy->setPosY(m_posBoundingSprite.y());
        });
        countY = 0;
    }
}

void EnemyGroup::timerEvent(QTimerEvent*)
{
    m_row = qrand() % m_enemy_.size();
    m_column = qrand() % m_enemy_[m_row].size();
    m_enemy_[m_row][m_column]->attack();
}
