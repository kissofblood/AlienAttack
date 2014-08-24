#ifndef HELPENEMY_H
#define HELPENEMY_H

#include "enemy.h"
#include <QGraphicsItem>
#include <QVector>
#include <QPropertyAnimation>
#include <QPointF>
#include <QTimer>
#include <QObject>

#include <QtWidgets>

class HelpEnemy : public QGraphicsPixmapItem
{
public:
    HelpEnemy(QGraphicsItem* parent = nullptr);
    ~HelpEnemy() override;

    void animationHelp_1(Enemy* enemy, const QPoint& pos);

private:
    QVector<QPixmap>    m_pixEnemy_;
    QTimer              *m_animHelp = new QTimer;
};

#endif // HELPENEMY_H
