#include "blackhole.h"

blackHole::blackHole(qreal posX, qreal posY, qreal r, qreal power):
    TableItem(posX, posY, r, 0,0)
{
    setPos(startX, startY);
    d = this->getRadius()*2;
    this->power = power;

}

blackHole::~blackHole()
{}


void blackHole::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // make the blackhole look like a bunch of circles drawn with gap between one another
    while(d < 256)
    {
        int delta = abs( ( ((int)this->radius * 2) % 128 ) - d/2);
        int alpha = 255 - (delta * delta) / 4 - d;

        if (alpha > 0)
        {
            painter->setPen(QPen(QColor(0, d/2, 127, alpha), 3));
            painter->drawEllipse(boundingRect());
        }
        d += 9;
    }
    d = this->getRadius()*2;

}

void blackHole::advance(int phase)
{
    if(!phase)
        return;
    update();
}

blackHole &blackHole::clone() const
{
    return *(new blackHole(*this));
}

QRectF blackHole::boundingRect() const
{
    return QRectF(-d/2,-d/2,d,d);
}
