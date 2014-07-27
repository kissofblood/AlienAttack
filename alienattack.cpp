#include "alienattack.h"

AlienAttack::AlienAttack(QWidget* parent) : QGraphicsView(parent)
{
    this->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    this->setScene(m_titlescreen);
    this->setFixedSize(1000, 620);

    this->connect(m_titlescreen, &TitleScreen::startGame, [this]()
    {
        this->setScene(m_battlefield);
        delete m_titlescreen;
    });
}
