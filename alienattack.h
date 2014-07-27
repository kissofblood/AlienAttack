#ifndef ALIENATTACK_H
#define ALIENATTACK_H

#include "titlescreen.h"
#include "battlefield.h"
#include <QWidget>
#include <QGraphicsView>
#include <QRectF>

class AlienAttack : public QGraphicsView
{
    Q_OBJECT

public:
    AlienAttack(QWidget* parent = nullptr);
    ~AlienAttack() = default;

private:
    TitleScreen             *m_titlescreen  = new TitleScreen(QRectF(0, 0, 995, 615), this);
    Battlefield             *m_battlefield  = new Battlefield(QRectF(0, 0, 995, 615), this);
};

#endif // ALIENATTACK_H
