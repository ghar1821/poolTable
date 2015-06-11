#ifndef PHYSICSPROTOTYPE_H
#define PHYSICSPROTOTYPE_H
#include <QGraphicsItem>
//Prototype for items which can be placed on the table, and have physical properties
class PhysicsPrototype : public QGraphicsItem
{
public:
    PhysicsPrototype();
    virtual PhysicsPrototype & clone() const = 0;
    virtual QRectF boundingRect() const = 0;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) = 0;
    virtual void advance(int phase) = 0;
    virtual qreal getRadius() = 0;


};

#endif // PHYSICSPROTOTYPE_H
