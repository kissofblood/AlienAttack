#include "battlefield.h"

Battlefield::Battlefield(const QRectF& rect, QObject* parent) : QGraphicsScene(rect, parent)
    , m_player(new Player(QPoint(300, 550), rect.right(), QPixmap(":player/resource/player.png")))
{
    this->addPixmap(QPixmap(":/titlescreen/resource/bgScreen.png"));
    this->addItem(m_player);

    this->addRect(rect);
}
