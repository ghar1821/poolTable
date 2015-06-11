#ifndef RESIDUE_H
#define RESIDUE_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QtCore/qmath.h>
#include <QDebug>
#include "table.h"
#include "tableitem.h"
#include "block.h"
#include "blackhole.h"

#include <QVector2D>


// residue class is the residues occur when the ball exploded.
// take a shape of mini ball.
// the reason for this class instead of just spawning multiple tiny balls is to allow flexibility of adding ball of size residue
// but behaving like normal ball.
// Besides, logically, residue shouldn't be balls. They are pieces of balls.
// Balls rollover residues and lose more speed.
class residue : public TableItem
{
public:
    residue(qreal posX, qreal posY,
            qreal r,
            qreal VX, qreal VY, table *table1 = 0);

    ~residue();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void advance(int phase);

    virtual residue & clone() const;

    bool CheckInPocketRange(QList<pocket *>pl);

private:
    qreal angle;
    table *t;
};

#endif // RESIDUE_H
