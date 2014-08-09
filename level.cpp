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
    Q_UNUSED(amount);
}

void Level::finishLevel()
{
    m_amount = 0;
    m_txtItemAmount->setPlainText(m_txtAmount.arg(QString::number(m_amount)));
    m_txtItemAmount->show();
    emit victory();

    QTimer::singleShot(1000, this, SLOT(activateLevel()));
}

void Level::activateLevel()
{
    m_speedShotEnemy(m_initialDataFun.first -= 1);
    m_speedEnemy(m_initialDataFun.second -= 1);
    m_activateTimer();
    emit nextLevel();

    qDebug()<<m_initialDataFun;
}
