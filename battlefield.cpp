#include "battlefield.h"

Battlefield::Battlefield(const QRectF& rect, QObject* parent) : QGraphicsScene(rect, parent)
    , m_player(new Player(QPoint(300, 550), rect.right(), QPixmap(":player/resource/player.png")))
{
    //this->addPixmap(QPixmap(":/titlescreen/resource/bgScreen.png"));
    m_itemPixLive = this->addPixmap(QPixmap(":player/resource/player.png"));
    m_itemPixLive->setPos(20, 550);
    m_itemTxtLive = this->addText("x3");
    m_itemTxtLive->setPos(m_itemPixLive->boundingRect().right() + 25, 550);
    m_itemTxtLive->setFont(QFont("Times", 25, QFont::Normal));
    m_itemTxtLive->setDefaultTextColor(QColor(Qt::white));

    QGraphicsTextItem* itemWordScope = this->addText("Time");
    itemWordScope->setPos(20, 20);
    itemWordScope->setFont(QFont("Times", 20, QFont::Normal));
    itemWordScope->setDefaultTextColor(QColor(Qt::white));
    m_itemTxtTimer = this->addText(QTime(0, 3, 0).toString("mm:ss"));
    m_itemTxtTimer->setPos(20, 50);
    m_itemTxtTimer->setFont(QFont("Times", 25, QFont::Normal));
    m_itemTxtTimer->setDefaultTextColor(QColor(Qt::white));

    m_enemyGroup = new EnemyGroup(QPoint(300, 20), rect.right(), m_player);

    m_helpEnemy->setPos(100, 300);
    m_enemyGroup->setAnimation(m_helpEnemy);
    this->addItem(m_helpEnemy);

    this->addItem(m_enemyGroup);
    this->addItem(m_player);
    this->connect(m_player,     &Player::fire,           this, &Battlefield::shot);
    this->connect(m_enemyGroup, &EnemyGroup::pathShot,   this, &Battlefield::collidingPlayer);
    this->connect(m_enemyGroup, &EnemyGroup::killEnemy,  this, &Battlefield::killEnemy);
    this->connect(m_enemyGroup, &EnemyGroup::gameOver,   this, &Battlefield::stopGame);
    this->connect(m_timer,      &QTimer::timeout,        this, &Battlefield::countdown);
}

void Battlefield::setSpeedShotEnemy(int msec)
{ m_enemyGroup->setSpeed(msec); }

void Battlefield::setSpeedEnemy(int msec)
{ m_enemyGroup->setSpeedEnemy(msec); }

void Battlefield::activateTime()
{
    m_timer->start(1000);
    m_player->setSpeed(8);
}

void Battlefield::shot(const QPoint& pos)
{
    const int countPix = 2;
    QVector<QPixmap> vecPix;
    for(int i = 0; i < countPix; i++)
        vecPix.push_back(QPixmap(QString(":shot/playershot/resource/shot/pshot_%1.png").arg(QString::number(i))));

    m_shot_.push_back(new Shot(Common::Person::Player, pos, 550, vecPix));
    this->addItem(m_shot_.back());
    this->connect(m_shot_.back(), &Shot::deleteShot, this, &Battlefield::deleteShotItem);
    this->connect(m_shot_.back(), &Shot::pathShot,   this, &Battlefield::collidingEnemy);
}

void Battlefield::deleteShotItem(Shot* shotItem)
{
    m_shot_.erase(std::remove(m_shot_.begin(), m_shot_.end(), shotItem), m_shot_.end());
    this->removeItem(shotItem);
    delete shotItem;
}

void Battlefield::reduceLife()
{
    int num = m_itemTxtLive->toPlainText().split("x").back().toInt() - 1;
    if(num == 1)
    {
        QGraphicsBlurEffect* blur = new QGraphicsBlurEffect(this);
        m_itemPixLive->setGraphicsEffect(blur);
        blur->setBlurRadius(0);

        QState* state1 = new QState(m_stateMachine);
        state1->assignProperty(blur, "blurRadius", 10);

        QState* state2 = new QState(m_stateMachine);
        state2->assignProperty(blur, "blurRadius", 0);

        QPropertyAnimation* anim = new QPropertyAnimation(blur, "blurRadius", this);
        anim->setDuration(1400);

        state1->addTransition(this, SIGNAL(transitionState2()), state2);
        state2->addTransition(this, SIGNAL(transitionState1()), state1);

        this->connect(blur, &QGraphicsBlurEffect::blurRadiusChanged, [this](int radius)
        {
            if(radius == 10)
                emit transitionState2();
            else if(radius == 0)
                emit transitionState1();
        });

        m_stateMachine->addDefaultAnimation(anim);
        m_stateMachine->setInitialState(state1);
        m_stateMachine->start();
    }
    else if(num == 0)
    {
        m_stateMachine->stop();
        m_player->explosion();
        stopGame();
    }
    m_itemTxtLive->setPlainText("x" + QString::number(num));
}

void Battlefield::collidingPlayer(Shot* shot)
{
    if(m_player->collidesWithItem(shot))
    {
        m_enemyGroup->removeShotItem(shot);
        deleteShotItem(shot);
        reduceLife();
    }
}

void Battlefield::collidingEnemy(Shot* shot)
{
    if(m_enemyGroup->collidingEnemy(shot))
        deleteShotItem(shot);
}

void Battlefield::countdown()
{
    QTime time = QTime::fromString(m_itemTxtTimer->toPlainText(), "mm:ss");
    if(time == QTime(0, 0, 0))
    {
        emit finishGame();
        m_timer->stop();
        return;
    }
    m_itemTxtTimer->setPlainText(QTime::fromMSecsSinceStartOfDay(time.msecsSinceStartOfDay() - 1000)
                                 .toString("mm:ss"));
}

void Battlefield::stopGame()
{
    m_player->stopGame();
    m_enemyGroup->stopGame();
    m_timer->stop();
    std::for_each(m_shot_.begin(), m_shot_.end(), std::bind(&Shot::stopGame, std::placeholders::_1));
    emit gameOver();
    this->update();
}
