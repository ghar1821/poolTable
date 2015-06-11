#ifndef BALLBUILDER_H
#define BALLBUILDER_H
#include "ball.h"
//A class to create Ball objects. Many ball objects that are exactly the same can be made with getManyCopies, useful for assignment 4
class BallFactory
{
public:
    BallFactory();
    ball * get(qreal X, qreal Y, qreal r, qreal vx, qreal vy, table * t=0);
    virtual ~BallFactory()
    {}
    QList<ball *> getManyCopies(ball * original, int numCopies);
};

#endif // BALLBUILDER_H
