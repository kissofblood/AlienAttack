#include "alienattack.h"

AlienAttack::AlienAttack(QWidget* parent) : QGraphicsView(parent)
{
    m_titlescreen->setSceneRect(0, 0, 980, 600);
    m_battlefield->setSceneRect(0, 0, 980, 600);
    this->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    this->setScene(m_titlescreen);
    this->setFixedSize(1000, 620);

    this->connect(m_titlescreen, &TitleScreen::startGame, this, &AlienAttack::loadGame);
}

void AlienAttack::loadGame()
{
    this->setScene(m_battlefield);
    delete m_titlescreen;
}
