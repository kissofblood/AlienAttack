#ifndef HELPENEMY_H
#define HELPENEMY_H

#include "enemy.h"
#include <QGraphicsItem>
#include <QVector>
#include <QPropertyAnimation>
#include <QPointF>
#include <QTimer>

#include <QtWidgets>

class HelpEnemy : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    HelpEnemy(QGraphicsItem* parent = nullptr);
    ~HelpEnemy() = default;

    void animationHelp_1(Enemy* enemy, const QPoint& pos, bool deleteAnim);

private slots:
    void animEnemy();

private:
    QVector<QPixmap>    m_pixEnemy_;
};

#endif // HELPENEMY_H
