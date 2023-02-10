#ifndef RIGHTBLINKERANIMATION_H
#define RIGHTBLINKERANIMATION_H
#include <QGraphicsItem>
#include <QPainter>
#include <QPixmap>
#include <QTimer>

class RightBlinkerAnimation : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    enum { Type = UserType + 2};
    RightBlinkerAnimation(QGraphicsItem *parent = nullptr);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int type() const;

public slots:
     void nextFrame();

private:
    int currentFrame;
    QPixmap iPixMap;

};
#endif // RIGHTBLINKERANIMATION_H
