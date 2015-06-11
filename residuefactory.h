#ifndef RESIDUEFACTORY_H
#define RESIDUEFACTORY_H
#include "residue.h"

class residueFactory
{
public:
    residueFactory();
    residue *get(qreal X, qreal Y, qreal r, qreal vx, qreal vy, table *t = 0);
    virtual ~residueFactory();
    QList<residue *> getManyCopies(residue *original, int numCopies);
};

#endif // RESIDUEFACTORY_H
