#include "ballfactory.h"



BallFactory::BallFactory()
{
}

ball * BallFactory::get(qreal X, qreal Y, qreal r, qreal vx, qreal vy, table * t)
{
        return new ball( X, Y, r, vx, vy,  t);
}


//returns a list of balls all of the same parameters, useful for assignment4
//returns an empty list if numCopies <= 0
QList<ball *> BallFactory::getManyCopies(ball * original, int numCopies)
{
    QList<ball *> list = QList<ball *>();
    for (int i = 0 ; i < numCopies; i++ )
    {
        list.append(&(original->clone()));
    }
    return list;

}
