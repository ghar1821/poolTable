#include "residuefactory.h"

residueFactory::residueFactory()
{}

residue *residueFactory::get(qreal X, qreal Y, qreal r, qreal vx, qreal vy, table *t)
{
    return new residue(X, Y, r, vx, vy, t);
}

residueFactory::~residueFactory()
{}

QList<residue *> residueFactory::getManyCopies(residue *original, int numCopies)
{
    QList<residue *> list = QList<residue *>();
    for(int i = 0; i != numCopies; ++i)
        list.append((&(original->clone() )));
    return list;
}
