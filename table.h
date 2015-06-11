#ifndef TABLE_H
#define TABLE_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>

#include <QList>
#include "pocket.h"

using namespace std;

class table
{
public:
    table();
    table(qreal width, qreal height, qreal penWidth,
            qreal tableFriction, qreal cushionFriction, QList<pocket *> pocketL);
    qreal getWidth();
    qreal getHeight();
    qreal getPenWidth();
    qreal getTableFriction();
    qreal getCushionFriction();
    QList<pocket*> getPocketList();

private:
    qreal width;
    qreal height;
    qreal penWidth;
    qreal tableFriction;
    qreal cushionFriction;
    QList<pocket*> pocketList;
};
#endif // TABLE_H
