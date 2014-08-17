#include "alienattack.h"

AlienAttack::AlienAttack(QWidget* parent) : QGraphicsView(parent)
{
    this->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    this->setScene(m_titlescreen);

    this->connect(m_titlescreen, &TitleScreen::startGame, this, &AlienAttack::setSceneBattlefield);
    this->connect(m_level, &Level::victory, this, &AlienAttack::restartSceneBattlefield);
    this->connect(m_level, &Level::nextLevel, [this]()
    {
        this->setScene(m_battlefield);
        this->connect(m_battlefield, &Battlefield::killEnemy,  m_level, &Level::amountKillEnemy, Qt::QueuedConnection);
        this->connect(m_battlefield, &Battlefield::finishGame, m_level, &Level::finishLevel);
        this->connect(m_battlefield, &Battlefield::gameOver, this, &AlienAttack::showText);
    });
}

void AlienAttack::setSceneBattlefield()
{
    m_battlefield = new Battlefield(QRectF(0, 0, Common::sizeScene.width(), Common::sizeScene.height()), this);
    //m_battlefield->setSpeedShotEnemy(3000);
    m_battlefield->setSpeedEnemy(25);
    m_battlefield->activateTime();
    m_level->initialData(3000, 25);
    m_level->setFun(std::make_tuple(
                        std::bind(&Battlefield::setSpeedShotEnemy, m_battlefield, std::placeholders::_1),
                        std::bind(&Battlefield::setSpeedEnemy,     m_battlefield, std::placeholders::_1),
                        std::bind(&Battlefield::activateTime,      m_battlefield)));
    this->setScene(m_battlefield);
    this->connect(m_battlefield, &Battlefield::killEnemy,  m_level, &Level::amountKillEnemy, Qt::QueuedConnection);
    this->connect(m_battlefield, &Battlefield::finishGame, m_level, &Level::finishLevel);
    this->connect(m_battlefield, &Battlefield::gameOver, this, &AlienAttack::showText);
    delete m_titlescreen;
}

void AlienAttack::restartSceneBattlefield()
{
    this->setScene(m_level);
    delete m_battlefield;
    m_battlefield = new Battlefield(QRectF(0, 0, Common::sizeScene.width(), Common::sizeScene.height()), this);
    m_level->setFun(std::make_tuple(
                        std::bind(&Battlefield::setSpeedShotEnemy, m_battlefield, std::placeholders::_1),
                        std::bind(&Battlefield::setSpeedEnemy,     m_battlefield, std::placeholders::_1),
                        std::bind(&Battlefield::activateTime,      m_battlefield)));
}

void AlienAttack::showText()
{
    QGraphicsTextItem* txtGameOver = m_battlefield->addText("Game Over");
    txtGameOver->setPos(this->width() / 2 - 100, 20);
    txtGameOver->setFont(QFont("Times", 60, QFont::Bold));
    txtGameOver->setDefaultTextColor(QColor(Qt::red));

    QTimer::singleShot(2000, this, SLOT(showResult()));
}

void AlienAttack::showResult()
{
    this->setScene(m_level);
    m_level->showResultGameOver();
}
