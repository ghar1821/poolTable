#include "table.h"

table::table()
{
    this->width = 300;
    this->height = 500;
    this->penWidth = 10;
    this->tableFriction = 0.99;
    this->cushionFriction = 0.85;
}

table::table(qreal w, qreal h, qreal pw, qreal tf, qreal cf, QList<pocket*> pocketL)
{
    this->width = w;
    this->height = h;
    this->penWidth = pw;
    this->tableFriction = tf;
    this->cushionFriction = cf;
    this->pocketList = pocketL;
}

qreal table::getWidth()
{
    return width;
}

qreal table::getHeight()
{
    return height;
}

qreal table::getPenWidth()
{
    return penWidth;
}

qreal table::getTableFriction()
{
    return tableFriction;
}

qreal table::getCushionFriction()
{
    return cushionFriction;

}

QList<pocket *> table::getPocketList()
{
    return pocketList;
}


