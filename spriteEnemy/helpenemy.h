#ifndef HELPENEMY_H
#define HELPENEMY_H

#include <QGraphicsItem>
#include <QVector>
#include <QPropertyAnimation>
#include <QPointF>
#include <QTimer>

#include <QtWidgets>

class HelpEnemy : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    HelpEnemy(QGraphicsItem* parent = nullptr);
    ~HelpEnemy() = default;

    void animationHelp1(const QPointF& pos);

private slots:
    void animEnemy();

private:
    QVector<QPixmap>        m_pixEnemy_;
    QTimer                  *m_animEnemy = new QTimer(this);
};

#endif // HELPENEMY_H
