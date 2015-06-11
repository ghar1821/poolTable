#ifndef BLOCK_H
#define BLOCK_H
#include "tableitem.h"
#include <QVector2D>
#include <QMessageBox>
#include <QDebug>
class Block : public TableItem
{
public:
    Block(qreal x, qreal y, qreal width, qreal height);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void advance(int phase);
    qreal getXVel(){ return 0;}
    qreal getYVel(){ return 0;}
    qreal getWidth() { return width;}
    qreal getHeight() {return height;}
    virtual Block & clone() const;

private:
    qreal width;
    qreal height;
};

#endif // BLOCK_H
