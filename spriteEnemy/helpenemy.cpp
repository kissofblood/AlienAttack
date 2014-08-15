#include "helpenemy.h"

HelpEnemy::HelpEnemy(QGraphicsItem* parent) : QObject(nullptr), QGraphicsPixmapItem(parent)
{
    const int countPix = 10;
    for(int i = 0; i < countPix; i++)
        m_pixEnemy_.push_back(QPixmap(QString(":enemy/resource/enemy/enemy1_%1").arg(QString::number(i))));

    this->connect(m_animEnemy, &QTimer::timeout, this, &HelpEnemy::animEnemy);
    m_animEnemy->start(70);
}

void HelpEnemy::animationHelp1(const QPointF& pos)
{
    QPropertyAnimation* anim = new QPropertyAnimation(this, "pos");
    anim->setDuration(1000);
    anim->setEndValue(pos);
    anim->setEasingCurve(QEasingCurve::Linear);

    anim->start(QAbstractAnimation::DeleteWhenStopped)  ;
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
