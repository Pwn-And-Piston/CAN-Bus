#ifndef LEFTBLINKERANIMATION_H
#define LEFTBLINKERANIMATION_H
#include <QGraphicsItem>
#include <QPainter>
#include <QPixmap>
#include <QTimer>

class LeftBlinkerAnimation : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    enum { Type = UserType + 1};
    LeftBlinkerAnimation(QGraphicsItem *parent = nullptr);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int type() const;

public slots:
     void nextFrame();

private:
    int currentFrame;
    QPixmap iPixMap;

};
#endif // LEFTBLINKERANIMATION_H
