#include "battlefield.h"

Battlefield::Battlefield(const QRectF& rect, QObject* parent) : QGraphicsScene(rect, parent)
    , m_player(new Player(QPoint(300, 550), rect.right(), QPixmap(":player/resource/player.png")))
{
    this->addPixmap(QPixmap(":/titlescreen/resource/bgScreen.png"));
    m_itemPixLive = this->addPixmap(QPixmap(":player/resource/player.png"));
    m_itemPixLive->setPos(20, 550);
    m_itemTxtLive = this->addText("x3");
    m_itemTxtLive->setPos(m_itemPixLive->boundingRect().right() + 25, 550);
    m_itemTxtLive->setFont(QFont("Times", 25, QFont::Normal));
    m_itemTxtLive->setDefaultTextColor(QColor(Qt::white));

    QGraphicsTextItem* itemWordScope = this->addText("Scope");
    itemWordScope->setPos(20, 20);
    itemWordScope->setFont(QFont("Times", 20, QFont::Normal));
    itemWordScope->setDefaultTextColor(QColor(Qt::white));
    m_itemTxtScope = this->addText("0");
    m_itemTxtScope->setPos(20, 50);
    m_itemTxtScope->setFont(QFont("Times", 25, QFont::Normal));
    m_itemTxtScope->setDefaultTextColor(QColor(Qt::white));

    const int countPix = 10;
    QVector<QPixmap> vecPix;
    for(int i = 0; i < countPix; i++)
        vecPix.push_back(QPixmap(QString(":enemy/resource/enemy/enemy1_%1").arg(QString::number(i))));
    m_enemy = new Enemy(QPoint(300, 20), rect.right(), vecPix);

    this->addItem(m_player);
    this->addItem(m_enemy);

    this->connect(m_player, &Player::fire, this, &Battlefield::shot);

    this->connect(m_enemy, &Enemy::fire, this, &Battlefield::funShot);
}

void Battlefield::shot(const QPoint& pos)
{
    const int countPix = 2;
    QVector<QPixmap> vecPix;
    for(int i = 0; i < countPix; i++)
        vecPix.push_back(QPixmap(QString(":shot/playershot/resource/shot/pshot_%1.png").arg(QString::number(i))));

    m_shot_.push_back(new Shot(AACommon::Person::Player, pos, 550, vecPix));
    this->addItem(m_shot_.back());
    this->connect(m_shot_.back(), &Shot::deleteShot, this, &Battlefield::deleteShotItem);
}

void Battlefield::deleteShotItem()
{
    Shot* shotItem = qobject_cast<Shot*>(this->sender());
    if(shotItem == nullptr)
        return;

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
        emit gameOver();
        this->update();
    }
    m_itemTxtLive->setPlainText("x" + QString::number(num));
}

void Battlefield::setScope()
{
    int num = m_itemTxtScope->toPlainText().toInt() + 1;
    m_itemTxtScope->setPlainText(QString::number(num));
}
