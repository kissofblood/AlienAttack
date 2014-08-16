#ifndef HELPENEMY_H
#define HELPENEMY_H

#include <QGraphicsItem>
#include <QVector>
#include <QPropertyAnimation>
#include <QPointF>
#include <QTimer>

#include <QtWidgets>

#include "enemy.h"

class HelpEnemy : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    HelpEnemy(QGraphicsItem* parent = nullptr);
    ~HelpEnemy() = default;

    void animationHelp_1(Enemy* enemy, const QPointF& pos);

signals:
    void showNewEnemy(Enemy* enemy);
    void insertNewEnemy(Enemy* enemy);

private slots:
    void animEnemy();

private:
    QVector<QPixmap>    m_pixEnemy_;
};

#endif // HELPENEMY_H
