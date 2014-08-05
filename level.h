#ifndef LEVEL_H
#define LEVEL_H

#include <QObject>
#include <QGraphicsScene>
#include <QRectF>

class Level : public QGraphicsScene
{
    Q_OBJECT
public:
    Level(const QRectF& rect, QObject* parent = nullptr);
    ~Level() = default;

signals:

public slots:

};

#endif // LEVEL_H
