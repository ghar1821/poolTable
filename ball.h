#ifndef BALL_H
#define BALL_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QtCore/qmath.h>
#include <QDebug>
#include <QVector2D>
#include <QtMultimedia/QSound>
#include <stdio.h>


#include <math.h>

#include "table.h"
#include "tableitem.h"
#include "pocket.h"
#include "residue.h"
#include "block.h"
#include "blackhole.h"

class ball : public TableItem
{
public:
    ball(qreal posX, qreal posY, qreal r, qreal VX, qreal VY, table *table1 = 0);
    ~ball();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void advance(int phase);

    bool CheckInPocketRange(QList<pocket *> pl);
    void addTable(table * newTable);
    void setExplosionSpeed(double spd);
    virtual ball & clone() const;
    void explode();

    table * getTable();

    QSound *chomp;

private:
    bool playBlackHoleSound;
    float angle;    // for blackhole. Initially initialised at 0 until it approaches blackhole
    table *t;
    double explosionSpeed;  // speed for a ball to explode



};

#endif // BALL_H
