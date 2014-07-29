#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QObject>
#include <QPixmap>
#include <QFont>
#include <QColor>
#include <QTimerEvent>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QRectF>
#include <QtMultimedia/QSound>

class TitleScreen : public QGraphicsScene
{
    Q_OBJECT
public:
    TitleScreen(const QRectF& rect, QObject* parent = nullptr);
    ~TitleScreen() = default;

signals:
    void startGame();

private:
    QGraphicsTextItem       *m_itemTxt = nullptr;
    QSound                  *m_sound   = new QSound(":/sound/resource/sound/theme.wav", this);
    int                     m_timerId  = -1;

    void timerEvent(QTimerEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    void stop();
};

#endif // TITLESCREEN_H
