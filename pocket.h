#ifndef POCKET_H
#define POCKET_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "tableitem.h"

class pocket : public TableItem
{
public:
    pocket(qreal posX, qreal posY, qreal r, qreal VX, qreal VY);
    pocket();
    ~pocket();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void advance(int phase);

    virtual pocket & clone() const;

};

#endif // POCKET_H
