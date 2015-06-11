#include "pocket.h"
#include <QDebug>

pocket::pocket(qreal posX, qreal posY, qreal r, qreal VX, qreal VY):
    TableItem(posX, posY, r, VX, VY)
{
    setPos(startX, startY);
    setZValue(-50); //Make sure the pockets are underneath the balls
}

pocket::~pocket()
{
}

pocket & pocket::clone() const
{

    return  *(new pocket(*this));
}

QRectF pocket::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(-radius - penWidth / 2, -radius - penWidth / 2,
                  radius*2 + penWidth, radius*2 + penWidth);
}

void pocket::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::black);

    painter->drawEllipse(boundingRect());
}

void pocket::advance(int phase)
{

}


