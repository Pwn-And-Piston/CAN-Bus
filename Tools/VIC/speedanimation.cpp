#include "speedanimation.h"

SpeedAnimation::SpeedAnimation(QGraphicsItem *parent): QGraphicsItem(parent) ,currentFrame() {

    setFlag(ItemClipsToShape);
    iPixMap = QPixmap(":/images/speed_animation.png");

    QTimer *timer = new QTimer(this);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(nextFrame()));
    timer->start(200);
}

void SpeedAnimation::nextFrame(){

    currentFrame += 85;
    if (currentFrame >= 926) {
        currentFrame = 0;
    }
}

QRectF SpeedAnimation::boundingRect() const {

    return QRectF(0,0,75,62);
}

void SpeedAnimation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    painter->drawPixmap(0,0, iPixMap, currentFrame, 0,75,62);
    setTransformOriginPoint(boundingRect().center());
    Q_UNUSED(widget);
    Q_UNUSED(option);
}

int SpeedAnimation::type() const {

    return Type;
}
