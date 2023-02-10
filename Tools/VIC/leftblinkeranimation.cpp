#include "leftblinkeranimation.h"

LeftBlinkerAnimation::LeftBlinkerAnimation(QGraphicsItem *parent): QGraphicsItem(parent) ,currentFrame() {

    setFlag(ItemClipsToShape);
    iPixMap = QPixmap(":/images/Left_Blink_Animation.png");

    QTimer *timer = new QTimer(this);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(nextFrame()));
    timer->start(100);
}

void LeftBlinkerAnimation::nextFrame(){

    currentFrame += 85;
    if (currentFrame >= 1606) {
        currentFrame = 0;
    }
}

QRectF LeftBlinkerAnimation::boundingRect() const {

    return QRectF(0,0,77,58);
}

void LeftBlinkerAnimation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    painter->drawPixmap(0,0, iPixMap, currentFrame, 0,77,58);
    setTransformOriginPoint(boundingRect().center());
    Q_UNUSED(widget);
    Q_UNUSED(option);
}

int LeftBlinkerAnimation::type() const {

    return Type;
}
