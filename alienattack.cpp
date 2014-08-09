#include "alienattack.h"

AlienAttack::AlienAttack(QWidget* parent) : QGraphicsView(parent)
{
    this->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    this->setScene(m_titlescreen);
    this->setFixedSize(1000, 620);

    this->connect(m_titlescreen, &TitleScreen::startGame, [this]()
    {
        m_battlefield = new Battlefield(QRectF(0, 0, Common::sizeScene.width(), Common::sizeScene.height()), this);
        m_battlefield->setSpeedShotEnemy(3000);
        m_battlefield->setSpeedEnemy(30);
        m_battlefield->activateTime();
        m_level->initialData(3000, 30);
        this->setScene(m_battlefield);
        this->connect(m_battlefield, &Battlefield::killEnemy,  m_level, &Level::amountKillEnemy);
        this->connect(m_battlefield, &Battlefield::finishGame, m_level, &Level::finishLevel);
        delete m_titlescreen;
    });
    this->connect(m_level, &Level::victory, [this]()
    {
        this->setScene(m_level);
        delete m_battlefield;
        m_battlefield = new Battlefield(QRectF(0, 0, Common::sizeScene.width(), Common::sizeScene.height()), this);
        m_level->setFun(std::make_tuple(
                            std::bind(&Battlefield::setSpeedShotEnemy, m_battlefield, std::placeholders::_1),
                            std::bind(&Battlefield::setSpeedEnemy, m_battlefield, std::placeholders::_1),
                            std::bind(&Battlefield::activateTime, m_battlefield)));
    });
    this->connect(m_level, &Level::nextLevel, [this]()
    {
        this->setScene(m_battlefield);
        this->connect(m_battlefield, &Battlefield::killEnemy,  m_level, &Level::amountKillEnemy);
        this->connect(m_battlefield, &Battlefield::finishGame, m_level, &Level::finishLevel);
    });
}
