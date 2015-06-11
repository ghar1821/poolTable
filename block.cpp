#include "block.h"

Block::Block(qreal x, qreal y, qreal width, qreal height) :
TableItem(x, y)
{
    this->startX = x;
    this->startY = y;
    this->width = width;
    this->height = height;
}

QRectF Block::boundingRect() const
{
    return QRectF(this->startX, this->startY, this->width, this->height);
}

void Block::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::red);
    painter->drawRect(boundingRect());
}

void Block::advance(int phase)
{
    if(!phase)
        return;
}

//return a copy of this object. Useful for using a Prototype
Block & Block::clone() const
{
    return  *(new Block(*this));
}
