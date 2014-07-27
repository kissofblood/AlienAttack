#ifndef ALIENATTACK_H
#define ALIENATTACK_H

#include "titlescreen.h"
#include "battlefield.h"
#include <QWidget>
#include <QGraphicsView>

class AlienAttack : public QGraphicsView
{
    Q_OBJECT

public:
    AlienAttack(QWidget* parent = nullptr);
    ~AlienAttack() = default;

private slots:
    void loadGame();

private:
    TitleScreen             *m_titlescreen  = new TitleScreen(this);
    Battlefield             *m_battlefield  = new Battlefield(this);
};

#endif // ALIENATTACK_H
