#include "enemygroup.h"

EnemyGroup::EnemyGroup(const QPoint& pos, int rightScene, Player* player, QGraphicsItem* parent) : AbstractSprite(parent)
    , m_posBoundingSprite(pos)
    , m_sizeBoundingSprite(rightScene - 5 - pos.x(), 240)
    , m_player(player)
{
    qsrand(QTime::currentTime().second());

    const int countPix = 10;
    QVector<QPixmap> vecPix;
    for(int i = 0; i < countPix; i++)
        vecPix.push_back(QPixmap(QString(":enemy/resource/enemy/enemy1_%1").arg(QString::number(i))));
    for(int i = 0, y = 0; i < m_row; i++, y += vecPix.front().height() + 10)
    {
        QVector<Enemy*> vecEnemy;
        for(int j = 0, x = 0; j < 7; j++, x += vecPix.front().width() + 5)
        {
            vecEnemy.push_back(new Enemy(QPoint(pos.x() + x, pos.y() + y), qMakePair(pos.x(), rightScene), vecPix));
            m_group->addToGroup(vecEnemy.back());
        }
        m_enemy_.push_back(vecEnemy);
    }

    std::for_each(m_enemy_.begin(), m_enemy_.end(), [this](const QVector<Enemy*>& vecEnemy)
    {
        for(Enemy* enemy1 : vecEnemy)
        {
            for(Enemy* enemy2 : vecEnemy)
                if(enemy1 != enemy2)
                    this->connect(enemy1, &Enemy::turnChange, enemy2, &Enemy::setTurn);
            this->connect(enemy1, &Enemy::fire,         this, &EnemyGroup::shot);
            this->connect(enemy1, &Enemy::turnChange,   this, &EnemyGroup::countDownEnemy);
        }
    });

    this->connect(m_randomShotEnemy, &QTimer::timeout, this, &EnemyGroup::randomShotEnemy);


    /*for(int i = 0; i < m_enemy_.size() - 1; i++)
    {
        for(int j = 0; j < m_enemy_[i].size(); j++)
        {
            m_enemy_[i][j]->hide();
        }
    }

    int i = m_enemy_.size() - 1;
    for(int j = 2; j < m_enemy_[i].size(); j++)
    {
        m_enemy_[i][j]->hide();
    }

    m_enemy_[i][0]->hide();*/
}

QRectF EnemyGroup::boundingRect() const
{
    return QRectF(m_posBoundingSprite.x(), m_posBoundingSprite.y(), m_sizeBoundingSprite.width(), m_sizeBoundingSprite.height());
}

void EnemyGroup::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    //Q_UNUSED(painter);
    painter->drawRect(boundingRect());
}

void EnemyGroup::setSpeed(int msec)
{
    if(msec > 0)
    {
        m_randomShotEnemy->stop();
        m_randomShotEnemy->start(msec);
    }
}

void EnemyGroup::setSpeedEnemy(int msec)
{
    std::for_each(m_enemy_.begin(), m_enemy_.end(), [msec](const QVector<Enemy*>& vecEnemy)
    {
       std::for_each(vecEnemy.begin(), vecEnemy.end(), std::bind(&Enemy::setSpeed, std::placeholders::_1, msec));
    });
    m_speedEnemy = msec;
}

void EnemyGroup::stopGame()
{
    m_randomShotEnemy->stop();
    std::for_each(m_enemy_.begin(), m_enemy_.end(), [](const QVector<Enemy*>& vecEnemy)
    {
        std::for_each(vecEnemy.begin(), vecEnemy.end(), std::bind(&Enemy::stopGame, std::placeholders::_1));
    });
    std::for_each(m_shot_.begin(), m_shot_.end(), std::bind(&Shot::stopGame, std::placeholders::_1));
}

void EnemyGroup::removeShotItem(Shot* shotItem)
{
    m_shot_.erase(std::remove(m_shot_.begin(), m_shot_.end(), shotItem), m_shot_.end());
    m_group->removeFromGroup(shotItem);
}

bool EnemyGroup::collidingEnemy(Shot* shot)
{
    static bool flagBackSize = false;
    for(int i = 0; i < m_enemy_.size(); i++)
        for(int j = 0; j < m_enemy_[i].size(); j++)
            if(m_enemy_[i][j]->collidesWithItem(shot))
            {
                m_group->removeFromGroup(m_enemy_[i][j]);
                delete m_enemy_[i][j];
                m_enemy_[i].remove(j);
                m_helpEnemy->start(m_speedEnemy);

                const int countPix = 10;
                QVector<QPixmap> vecPix;
                for(int i = 0; i < countPix; i++)
                    vecPix.push_back(QPixmap(QString(":enemy/resource/enemy/enemy1_%1").arg(QString::number(i))));
                m_enemy = new Enemy(m_posStartEnemy, vecPix, this);

                this->connect(m_helpEnemy, &QTimer::timeout,
                this, std::bind(&EnemyGroup::helpEnemy, this, m_enemy));

                if(m_enemy_[i].empty())
                {
                    if(i == m_enemy_.size() - 1)
                        flagBackSize = true;
                    m_enemy_.remove(i);
                }

                int amount = 0;
                std::for_each(m_enemy_.begin(), m_enemy_.end(), [&amount](const QVector<Enemy*>& vecEnemy)
                { amount += vecEnemy.size(); });
                emit killEnemy(amount);

                this->prepareGeometryChange();
                if(m_enemy_.size() < m_row && !m_enemy_.empty())
                {
                    if(flagBackSize)
                    {
                        m_sizeBoundingSprite.setHeight(m_sizeBoundingSprite.height() - m_enemy_.back()
                                                       .back()->boundingRect().height() - 10);
                        flagBackSize = false;
                    }
                    m_row = m_enemy_.size();
                }
                return true;
            }
    return false;
}

void EnemyGroup::setAnimation(HelpEnemy* helpEnemy)
{
    m_help = helpEnemy;
    m_posStartEnemy = QPoint(helpEnemy->pos().x(), helpEnemy->pos().y());
}

void EnemyGroup::showEnemy(Enemy* enemy)
{
    enemy->setParentItem(this);
}

void EnemyGroup::insertNemy(Enemy* enemy)
{
    Q_UNUSED(enemy);
    qDebug()<<"STOP";
    m_helpEnemy->stop();
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
    this->connect(m_shot_.back(), &Shot::pathShot,   this, &EnemyGroup::pathShot);
}

void EnemyGroup::deleteShotItem(Shot* shotItem)
{
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

    if(count >= m_enemy_.size())
    {
        countY += 1;
        count = 0;
    }

    if(countY == 10)
    {
        static int posY = m_posBoundingSprite.y();
        this->prepareGeometryChange();
        posY += 20;
        m_sizeBoundingSprite.setHeight(m_sizeBoundingSprite.height() + 20);
        std::for_each(m_enemy_.begin(), m_enemy_.end(), [this](const QVector<Enemy*>& vecEnemy)
        {
            std::for_each(vecEnemy.begin(), vecEnemy.end(), std::bind(
                              &Enemy::setPosYSpite, std::placeholders::_1, posY));
        });
        countY = count = 0;
        if(outputAbroad())
            emit gameOver();
    }
}

void EnemyGroup::randomShotEnemy()
{
    if(m_enemy_.empty())
    {
        m_randomShotEnemy->stop();
        return;
    }
    int row = qrand() % m_enemy_.size();
    int column = qrand() % m_enemy_[row].size();
    m_enemy_[row][column]->attack();
}

void EnemyGroup::helpEnemy(Enemy* enemy)
{
    int back = m_enemy_.size() - 1;
    // for(int i = 0, j = 1; i < m_enemy_.back().size(); i++)
     {
         int width = m_enemy_[back][1]->posSpiteTopLeft().x()
                     - m_enemy_[back][0]->posSpriteTopRight().x();
         if(width == 6)
         {
             return;
         }

         if(m_enemy_[back][0]->posSpriteTopRight().y() + 1 == m_enemy->posSpiteTopLeft().y())
         {
             qDebug()<<"STOP";
             m_helpEnemy->stop();
             m_enemy->operator =(*m_enemy_[back][0]);
             m_enemy_[back].push_back(m_enemy);


             this->connect(m_enemy_[back][0], &Enemy::turnChange, m_enemy, &Enemy::setTurn);
             this->connect(m_enemy_[back][m_enemy_[back].size() - 2
                  ], &Enemy::turnChange, m_enemy, &Enemy::setTurn);
             this->connect(m_enemy, &Enemy::fire,         this, &EnemyGroup::shot);
             this->connect(m_enemy, &Enemy::turnChange,   this, &EnemyGroup::countDownEnemy);

             return;
         }

         m_help->animationHelp_1(enemy, QPoint(m_enemy_[back][0]->posSpriteTopRight().x() + 5
                                 , m_enemy_[back][0]->posSpriteTopRight().y()));

     }
}

bool EnemyGroup::outputAbroad()
{
    return this->collidesWithItem(m_player, Qt::IntersectsItemBoundingRect);
}
