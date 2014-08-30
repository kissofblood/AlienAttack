#ifndef LEVEL_H
#define LEVEL_H

#include <QObject>
#include <QGraphicsScene>
#include <QRectF>
#include <QGraphicsTextItem>
#include <functional>
#include <tuple>
#include <QPair>
#include <QTimer>
#include <QString>
#include <QFont>
#include <QColor>

class Level : public QGraphicsScene
{
    Q_OBJECT

    using FunSpeed = std::function<void(int)>;
    using FunTimer = std::function<void()>;
public:
    Level(const QRectF& rect, QObject* parent = nullptr);
    ~Level() = default;

    void setFun(const std::tuple<FunSpeed, FunSpeed, FunTimer>& fun);
    void initialData(int speedShotEnemy, int speedEnemy);
    void showResultGameOver();

signals:
    void victory();
    void nextLevel();

public slots:
    void amountKillEnemy(int amount);
    void finishLevel();

private slots:
    void activateLevel();

private:
    int                         m_amount    = 0;
    int                         m_level     = 1;
    QPair<int, int>             m_initialDataFun;
    QString                     m_txtAmount = { "Amount Kill Enemy %1" };
    QGraphicsTextItem           *m_txtItemAmount    = nullptr;
    QGraphicsTextItem           *m_txtItemTimer     = nullptr;
    FunSpeed                    m_speedShotEnemy    = nullptr;
    FunSpeed                    m_speedEnemy        = nullptr;
    FunTimer                    m_activateTimer     = nullptr;
};

#endif // LEVEL_H
