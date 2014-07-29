#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "player.h"
#include "enemy.h"
#include "shot.h"
#include "common.h"
#include <QObject>
#include <QGraphicsScene>
#include <QPixmap>
#include <QPoint>
#include <QRectF>
#include <QVector>
#include <algorithm>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QFont>
#include <QColor>
#include <QGraphicsBlurEffect>
#include <QStateMachine>
#include <QState>
#include <QPropertyAnimation>
#include <QString>

class Battlefield : public QGraphicsScene
{
    Q_OBJECT
public:
    Battlefield(const QRectF& rect, QObject* parent = nullptr);
    ~Battlefield() = default;

signals:
    void transitionState1();
    void transitionState2();
    void gameOver();

private slots:
    void shot(const QPoint& pos);
    void deleteShotItem();
    void reduceLife();
    void setScope();


    void funShot(const QPoint& pos)
    {
        const int countPix = 2;
        QVector<QPixmap> vecPix;
        for(int i = 0; i < countPix; i++)
            vecPix.push_back(QPixmap(QString(":shot/enemy/resource/shot/eshot_%1.png").arg(QString::number(i))));

        m_vecShot.push_back(new Shot(AACommon::Person::Enemy, pos, this->height(), vecPix));
        this->addItem(m_vecShot.back());
        this->connect(m_vecShot.back(), &Shot::deleteShot, this, &Battlefield::deleteShotItem);
    }

    void deleteVecShot()
    {
        Shot* shotItem = qobject_cast<Shot*>(this->sender());
        if(shotItem == nullptr)
            return;

        m_vecShot.erase(std::remove(m_shot_.begin(), m_shot_.end(), shotItem), m_shot_.end());
        this->removeItem(shotItem);
        delete shotItem;

        qDebug()<<m_vecShot.size();
    }

private:
    Player              *m_player       = nullptr;
    Enemy               *m_enemy        = nullptr;
    QGraphicsTextItem   *m_itemTxtLive  = nullptr;
    QGraphicsPixmapItem *m_itemPixLive  = nullptr;
    QGraphicsTextItem   *m_itemTxtScope = nullptr;
    QStateMachine       *m_stateMachine = new QStateMachine(this);
    QVector<Shot*>      m_shot_;

    QVector<Shot*>      m_vecShot;
};

#endif // BATTLEFIELD_H
