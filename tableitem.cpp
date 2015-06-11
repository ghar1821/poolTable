#include "tableitem.h"

TableItem::TableItem(qreal posX, qreal posY, qreal r, qreal VX, qreal VY)
{
    this->xComponent = VX;
    this->yComponent = VY;

    this->startX = posX;
    this->startY = posY;

    this->radius = r;
}


TableItem::~TableItem()
{}


// checks if this item is in a valid position on the table
bool TableItem::validPosition(qreal width, qreal height)
{
    qreal x = getXPos();
    qreal y = getYPos();
    qreal r = getRadius();
    if(x + r < 0 || y + r< 0 || x + r > width || y + r > height)
        return false;
    return true;
}
bool TableItem::XOffBoundary(qreal tableW, qreal xPos)
{
    qreal penWidth = 10;
    if(xPos <= radius+penWidth/2 || xPos >= tableW-radius-penWidth/2)
    {
        return true;
    }
    return false;
}

bool TableItem::YOffBoundary(qreal tableH, qreal yPos)
{
    qreal penWidth = 10;
    if(yPos <= radius+penWidth/2 || yPos >= tableH-radius-penWidth/2)
    {
        return true;
    }
    return false;
}

void TableItem::FrictionMovement(qreal friction)
{
    xComponent *= friction;
    yComponent *= friction;
}

void TableItem::CushionEnergyLoss(qreal cFriction)
{
    xComponent *= cFriction;
    yComponent *= cFriction;
}

qreal TableItem::getXPos()
{
    return startX;
}

qreal TableItem::getYPos()
{
    return startY;
}

qreal TableItem::getRadius()
{
    return radius;
}


