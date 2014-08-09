#ifndef ALIENATTACK_H
#define ALIENATTACK_H

#include "titlescreen.h"
#include "battlefield.h"
#include "level.h"
#include "common.h"
#include <QWidget>
#include <QGraphicsView>
#include <QRectF>
#include <tuple>
#include <functional>

class AlienAttack : public QGraphicsView
{
    Q_OBJECT
public:
    AlienAttack(QWidget* parent = nullptr);
    ~AlienAttack() = default;

private:
    TitleScreen     *m_titlescreen  = new TitleScreen(QRectF(0, 0, Common::sizeScene.width(), Common::sizeScene.height()), this);
    Battlefield     *m_battlefield  = nullptr;
    Level           *m_level        = new Level(QRectF(0, 0, Common::sizeScene.width(), Common::sizeScene.height()), this);
};

#endif // ALIENATTACK_H
