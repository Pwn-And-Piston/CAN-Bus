#include "rightblinkeranimation.h"

RightBlinkerAnimation::RightBlinkerAnimation(QGraphicsItem *parent): QGraphicsItem(parent) ,currentFrame() {

    setFlag(ItemClipsToShape);
    iPixMap = QPixmap(":/images/Right_Blink_Animation.png");

    QTimer *timer = new QTimer(this);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(nextFrame()));
    timer->start(100);
}

void RightBlinkerAnimation::nextFrame(){

    currentFrame += 85;
    if (currentFrame >= 1606) {
        currentFrame = 0;
    }
}

QRectF RightBlinkerAnimation::boundingRect() const {

    return QRectF(0,0,77,58);
}

void RightBlinkerAnimation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    painter->drawPixmap(0,0, iPixMap, currentFrame, 0,77,58);
    setTransformOriginPoint(boundingRect().center());
    Q_UNUSED(widget);
    Q_UNUSED(option);
}

int RightBlinkerAnimation::type() const {

    return Type;
}
