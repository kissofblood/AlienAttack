#include "helpenemy.h"

HelpEnemy::HelpEnemy(QGraphicsItem* parent) : QObject(nullptr), QGraphicsPixmapItem(parent)
{
    const int countPix = 10;
    for(int i = 0; i < countPix; i++)
        m_pixEnemy_.push_back(QPixmap(QString(":enemy/resource/enemy/enemy1_%1").arg(QString::number(i))));

    QTimer* animHelp = new QTimer(this);
    this->connect(animHelp, &QTimer::timeout, this, &HelpEnemy::animEnemy);
    animHelp->start(70);
}

void HelpEnemy::animationHelp_1(Enemy* enemy, const QPoint& pos)
{
    if(enemy == nullptr)
    {
        return;
    }
    QPropertyAnimation* anim = new QPropertyAnimation(enemy, "scenePosSprite", this);
    anim->setDuration(100);
    anim->setEndValue(pos);
    anim->setEasingCurve(QEasingCurve::Linear);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void HelpEnemy::animEnemy()
{
    static int  frameIndex = 0;
    static bool flagForwardOrBack = true;
    if(frameIndex < m_pixEnemy_.size() && flagForwardOrBack)
    {
        frameIndex += 1;
        if(frameIndex == m_pixEnemy_.size() - 1)
            flagForwardOrBack = false;
    }
    else
    {
        frameIndex -= 1;
        if(frameIndex == 0)
            flagForwardOrBack = true;
    }
    this->setPixmap(m_pixEnemy_[frameIndex]);
}
