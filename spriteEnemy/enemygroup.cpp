#include "enemygroup.h"

EnemyGroup::EnemyGroup(const QPoint& pos, int rightScene, QGraphicsItem* parent) : AbstractSprite(parent)
    , m_posBoundingSprite(pos)
    , m_sizeBoundingSprite(rightScene - 5 - pos.x(), 530)
{
    qsrand(QTime::currentTime().second());

    const int countPix = 10;
    for(int i = 0; i < countPix; i++)
        m_pixEnemy.push_back(QPixmap(QString(":enemy/resource/enemy/enemy1_%1").arg(QString::number(i))));
    for(int i = 0, y = 0; i < m_row; i++, y += m_pixEnemy.front().height() + 10)
    {
        QVector<Enemy*> vecEnemy;
        for(int j = 0, x = 0; j < 7; j++, x += m_pixEnemy.front().width() + 5)
        {
            vecEnemy.push_back(new Enemy(QPoint(pos.x() + x, pos.y() + y), qMakePair(pos.x(), rightScene), m_pixEnemy));
            vecEnemy.back()->setObjectName(QString::number(i));
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

    this->connect(m_randHelpEnemy, &QTimer::timeout, this, &EnemyGroup::randomHelpEnemy);
    m_randHelpEnemy->start(500);
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
        m_randShotEnemy->stop();
        m_randShotEnemy->start(msec);
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
    m_randShotEnemy->stop();
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
    for(int i = 0; i < m_enemy_.size(); i++)
        for(int j = 0; j < m_enemy_[i].size(); j++)
            if(m_enemy_[i][j]->collidesWithItem(shot))
            {
                m_group->removeFromGroup(m_enemy_[i][j]);
                delete m_enemy_[i][j];
                m_enemy_[i].remove(j);
                if(m_enemy_[i].empty())
                    m_enemy_.remove(i);

                int amount = 0;
                std::for_each(m_enemy_.begin(), m_enemy_.end(), [&amount](const QVector<Enemy*>& vecEnemy)
                { amount += vecEnemy.size(); });
                emit killEnemy(amount);

                return true;
            }
    return false;
}

void EnemyGroup::setAnimation(HelpEnemy* helpEnemy)
{
    m_posStartEnemy = QPoint(helpEnemy->pos().x(), helpEnemy->pos().y());
    m_animationHelp_.push_back(std::bind(&HelpEnemy::animationHelp_1, helpEnemy,
                                         std::placeholders::_1, std::placeholders::_2));
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
    static int countDonw = 0;
    if(moveSprite == Common::MoveSprite::TurnRight)
        count += 1;

    if(count >= m_enemy_.size())
    {
        countDonw += 1;
        count = 0;
    }

    if(countDonw == 1)
    {
        std::for_each(m_enemy_.begin(), m_enemy_.end(), [this](const QVector<Enemy*>& vecEnemy)
        {
            std::for_each(vecEnemy.begin(), vecEnemy.end(), std::bind(&Enemy::downSprite, std::placeholders::_1));
        });
        countDonw = count = 0;
        if(outputAbroad())
            emit gameOver();
    }
}

void EnemyGroup::randomShotEnemy()
{
    if(m_enemy_.empty())
    {
        m_randShotEnemy->stop();
        return;
    }
    int row = qrand() % m_enemy_.size();
    int column = qrand() % m_enemy_[row].size();
    m_enemy_[row][column]->attack();
}

void EnemyGroup::randomHelpEnemy()
{
    if(m_enemy_.size() == 1)
    {
        m_insertHelpEnemy->stop();
        m_randHelpEnemy->stop();
        return;
    }

    QVector<Enemy*> leftEnemy;
    for(auto& vecEnemy : m_enemy_)
        for(int i = 1; i < vecEnemy.size(); i++)
        {
            int distance = vecEnemy[i]->posSpiteTopLeft().x() - vecEnemy[i - 1]->posSpriteTopRight().x();
            if(distance > 13)
                leftEnemy.push_back(vecEnemy[i - 1]);
        }

    if(leftEnemy.empty())
    {
        qDebug()<<"RETURN";
        m_randHelpEnemy->start(500);
        return;
    }

    m_randHelpEnemy->stop();
    int amountHelpEnemy = qrand() % leftEnemy.size();
    forever
    {
        QPointer<Enemy> currentEnemy = leftEnemy[qrand() % leftEnemy.size()];
        if(std::find_if(m_randNewEnemy_.begin(), m_randNewEnemy_.end(), [currentEnemy](const QPair<Enemy*, Enemy*>& pair)->bool
        { return pair.second == currentEnemy; }) == m_randNewEnemy_.end())
        {
            Enemy* newEnemy = new Enemy(m_posStartEnemy, m_pixEnemy, this);
            newEnemy->setObjectName(currentEnemy->objectName());
            m_randNewEnemy_.push_back(qMakePair(newEnemy, currentEnemy));
        }
        if(amountHelpEnemy == m_randNewEnemy_.size() - 1)
            break;
    }

    m_insertHelpEnemy->start(m_speedEnemy);
    this->connect(m_insertHelpEnemy, &QTimer::timeout, this, &EnemyGroup::insertHelpEnemy);
}

void EnemyGroup::insertHelpEnemy()
{
    static int count = 0;
    qDebug()<<"START"<<count++;
    for(int i = 0; i < m_randNewEnemy_.size(); i++)
    {
        if(m_randNewEnemy_[i].second != nullptr)
        {
            if(m_randNewEnemy_[i].first->posSpiteTopLeft().y() == m_randNewEnemy_[i].second->posSpriteTopRight().y() + 1
                    ||m_randNewEnemy_[i].first->posSpiteTopLeft().y() == m_randNewEnemy_[i].second->posSpriteTopRight().y() - 1
               || m_randNewEnemy_[i].first->posSpiteTopLeft().y() == m_randNewEnemy_[i].second->posSpriteTopRight().y())
            {
                (*m_randNewEnemy_[i].first) += m_randNewEnemy_[i].second;
                m_group->addToGroup(m_randNewEnemy_[i].first);
                for(auto& vecEnemy : m_enemy_)
                {
                    bool flagBreak = false;
                    for(int j = 0; j < vecEnemy.size(); j++)
                        if(vecEnemy[j] == m_randNewEnemy_[i].second)
                        {
                            vecEnemy.insert(j + 1, m_randNewEnemy_[i].first);

                            for(Enemy* enemyConnect : vecEnemy)
                                if(enemyConnect != m_randNewEnemy_[i].first)
                                {
                                    this->connect(enemyConnect, &Enemy::turnChange, m_randNewEnemy_[i].first, &Enemy::setTurn);
                                    this->connect(m_randNewEnemy_[i].first, &Enemy::turnChange, enemyConnect, &Enemy::setTurn);
                                }
                            this->connect(m_randNewEnemy_[i].first, &Enemy::fire,        this, &EnemyGroup::shot);
                            this->connect(m_randNewEnemy_[i].first, &Enemy::turnChange,  this, &EnemyGroup::countDownEnemy);

                            flagBreak = true;
                            break;
                        }
                    if(flagBreak)
                        break;
                }

                m_randNewEnemy_.removeAt(i);
                break;
            }
            else
            {
                QPoint point(m_randNewEnemy_[i].second->posSpriteTopRight().x() + 5, m_randNewEnemy_[i].second->posSpriteTopRight().y());
                m_animationHelp_[qrand() % m_animationHelp_.size()](m_randNewEnemy_[i].first, point);
            }
        }
        else
        {
            int index = m_randNewEnemy_[i].first->objectName().toInt();
            if(m_enemy_[index].isEmpty())
            {
                delete m_randNewEnemy_[i].first;
                m_randNewEnemy_.remove(i);
                break;
            }

            for(int j = 1; j < m_enemy_[index].size(); j++)
            {
                int min = m_enemy_[index][j - 1]->posSpriteTopRight().x();
                int val = m_randNewEnemy_[i].first->posSpiteTopLeft().x();
                int max = m_enemy_[index][j]->posSpiteTopLeft().x();
                int boundVal = qBound(min, val, max);
                if(boundVal == val)
                {
                    int distance1 = val - min;
                    int distance2 = max - val;
                    int minDistance = qMin(distance1, distance2);
                    if(minDistance == distance1)
                        m_randNewEnemy_[i].second = m_enemy_[index][j - 1];
                    else if(minDistance == distance2)
                        m_randNewEnemy_[i].second = m_enemy_[index][j];
                    break;
                }
                else
                {
                    delete m_randNewEnemy_[i].first;
                    m_randNewEnemy_.remove(i);
                    break;
                }
            }
        }
    }

    if(m_randNewEnemy_.empty())
    {
        m_insertHelpEnemy->stop();
        qDebug()<<"STOP";
        m_randHelpEnemy->start(500);
    }
}

bool EnemyGroup::outputAbroad()
{
    Enemy* enemy = m_enemy_.back().front();
    if(enemy->posSpiteTopLeft().y() + enemy->boundingRect().height() > boundingRect().bottom())
        return true;
    return false;
}
