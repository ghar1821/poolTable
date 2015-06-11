#ifndef TABLEITEM_H
#define TABLEITEM_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "physicsprototype.h"
class TableItem: public PhysicsPrototype

{
public:
    // Constructor
    TableItem(qreal posX, qreal posY, qreal r, qreal VX, qreal VY);
    TableItem(qreal x, qreal y) : startX(x), startY(y), xComponent(0), yComponent(0), radius(0){}
    TableItem(const TableItem & other) : startX(other.startX), startY(other.startY), radius(other.radius), xComponent(other.xComponent), yComponent(other.yComponent){}

public:
    // Other Functions
    virtual QRectF boundingRect() const = 0;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) = 0;
    virtual void advance(int phase) = 0;
    virtual ~TableItem();

    bool XOffBoundary(qreal tableW, qreal xPos);
    bool YOffBoundary(qreal tableH, qreal yPos);

    void FrictionMovement(qreal friction);
    virtual void CushionEnergyLoss(qreal cFriction);

    bool validPosition(qreal width, qreal height);
    virtual TableItem & clone() const = 0;

public:
    // Getter and setter
    qreal getXPos();
    qreal getYPos();
    virtual qreal getRadius();
    virtual qreal getXVel() { return xComponent;}
    virtual qreal getYVel() { return yComponent;}
    void setXVel (qreal xv) { xComponent = xv;}
    void setYVel (qreal yv) { yComponent = yv;}
    void setRadius (qreal r) { radius = r;}


protected:
    qreal startX;
    qreal startY;
    qreal radius;
    qreal xComponent;
    qreal yComponent;



};

#endif // TABLEITEM_H
