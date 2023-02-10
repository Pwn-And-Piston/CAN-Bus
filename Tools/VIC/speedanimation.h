#ifndef SPEEDANIMATION_H
#define SPEEDANIMATION_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPixmap>
#include <QTimer>

class SpeedAnimation : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    enum { Type = UserType + 3};
    SpeedAnimation(QGraphicsItem *parent = nullptr);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int type() const;

public slots:
     void nextFrame();

private:
    int currentFrame;
    QPixmap iPixMap;

};
#endif // SPEEDANIMATION_H
