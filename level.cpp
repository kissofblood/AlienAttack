#include "level.h"

Level::Level(const QRectF& rect, QObject* parent) : QGraphicsScene(rect, parent)
{
    //m_item = this->addPixmap(QPixmap(":/titlescreen/resource/bgScreen.png"));
    m_txtItemAmount = this->addText(m_txtAmount);
    m_txtItemAmount->setPos(rect.width() / 2 - 150, rect.height() / 2 - 100);
    m_txtItemAmount->setFont(QFont("Times", 20, QFont::Normal));
    m_txtItemAmount->setDefaultTextColor(QColor(Qt::black));
    m_txtItemAmount->hide();
}

void Level::setFun(const std::tuple<Level::FunSpeed, Level::FunSpeed, Level::FunTimer>& fun)
{ std::tie(m_speedShotEnemy, m_speedEnemy, m_activateTimer) = fun; }

void Level::initialData(int speedShotEnemy, int speedEnemy)
{ m_initialDataFun = qMakePair(speedShotEnemy, speedEnemy); }

void Level::amountKillEnemy(int amount)
{
    m_amount += 1;
    if(amount == 2)
    {
        m_initialDataFun.first = 2000;
        m_initialDataFun.second = 15;
        m_speedShotEnemy(m_initialDataFun.first);
        m_speedEnemy(m_initialDataFun.second);
    }
    else if(amount == 1)
    {
        m_initialDataFun.first = 1000;
        m_initialDataFun.second = 5;
        m_speedShotEnemy(m_initialDataFun.first);
        m_speedEnemy(m_initialDataFun.second);
    }
}

void Level::finishLevel()
{
    m_level += 1;
    m_txtItemAmount->setPlainText(m_txtAmount.arg(QString::number(m_amount)));
    m_txtItemAmount->show();
    emit victory();

    QTimer::singleShot(1000, this, SLOT(activateLevel()));
    m_amount = 0;
}

void Level::activateLevel()
{
    m_speedShotEnemy(m_initialDataFun.first -= 1);
    m_speedEnemy(m_initialDataFun.second);
    m_activateTimer();
    emit nextLevel();
}
