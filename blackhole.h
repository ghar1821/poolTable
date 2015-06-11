#ifndef BLACKHOLE_H
#define BLACKHOLE_H

#include <QDebug>
#include "tableitem.h"

class blackHole : public TableItem
{
public:
    blackHole(qreal posX, qreal posY, qreal r, qreal power);
    ~blackHole();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void advance(int phase);

    virtual blackHole &clone() const;
    qreal returnPower() { return power;}
private:
    int d;
    qreal power;

};

#endif // BLACKHOLE_H
