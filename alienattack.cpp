#include "alienattack.h"

AlienAttack::AlienAttack(QWidget* parent) : QGraphicsView(parent)
{
    //m_battlefield = new Battlefield(QRectF(0, 0, Common::sizeScene.width(), Common::sizeScene.height()), this);

    this->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    this->setScene(m_titlescreen);
    this->setFixedSize(1000, 620);

    this->connect(m_titlescreen, &TitleScreen::startGame, [this]()
    {
        m_battlefield = new Battlefield(QRectF(0, 0, Common::sizeScene.width(), Common::sizeScene.height()), this);
        this->setScene(m_battlefield);
        delete m_titlescreen;
    });
}
