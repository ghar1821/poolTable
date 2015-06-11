#include "residue.h"

residue::residue(qreal posX, qreal posY, qreal r, qreal VX, qreal VY, table *table1):
    TableItem(posX,posY,r,VX,VY),
    t(table1)
{
    setPos(this->startX,this->startY);
    this->angle = 0;
}

residue::~residue()
{
}

QRectF residue::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(-this->radius - penWidth / 2, -this->radius - penWidth / 2,
                      this->radius*2 + penWidth, this->radius*2 + penWidth);
}

void residue::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(!CheckInPocketRange(this->t->getPocketList()))
    {
        painter->setPen(Qt::black);
        painter->setBrush(Qt::white);
        painter->drawEllipse(boundingRect());
    }
    else
        delete this;
}

// residue collide with one another,
// get sucked into blackhole,
// entered pocket,
// bounce against blocks,
// but not against balls. Balls roll over them.
void residue::advance(int phase)
{
    if(!phase) return;

    QPointF next_point=mapToParent(xComponent,yComponent);
    qreal next_x = next_point.x();
    qreal next_y = next_point.y();
    qreal new_x = next_x;
    qreal new_y = next_y;

    qreal width = this->t->getWidth();
    qreal height = this->t->getHeight();

    qreal penWidth = this->t->getPenWidth();

    qreal widthBound = width-this->radius-penWidth/2;
    qreal heightBound = height-this->radius-penWidth/2;

    while(XOffBoundary(t->getWidth(), new_x) || YOffBoundary(t->getHeight(), new_y))
    {
        //right boundary
        if(next_x >= widthBound){
            new_x = 2*widthBound - next_x;
            xComponent= -xComponent;
        }
        //left boundary
        if(next_x <= (this->radius+penWidth/2)){
            new_x = 2*(this->radius+penWidth/2)-next_x;
            xComponent= -xComponent;
        }
        //bottom boundary
        if(next_y >= heightBound){
            new_y = 2*heightBound - next_y;
            yComponent= -yComponent;
        }
        //top boundary
        if(next_y <= (this->radius+penWidth/2)){
            new_y = 2*(this->radius+penWidth/2)-next_y;
            yComponent= -yComponent;
        }

        qreal cFriction = t->getCushionFriction();
        CushionEnergyLoss(cFriction);
    }

    QList<QGraphicsItem *> colliders = collidingItems();
    if(colliders.size() != 0)
    {
        qreal x = pos().x();
        qreal y = pos().y();
        QVector2D resVec = QVector2D(pos());
        QVector2D resVelocity = QVector2D(this->getXVel(), this->getYVel());
        //For every item that we collide with
        for(QList<QGraphicsItem *>::iterator iter = colliders.begin(); iter != colliders.end(); ++iter)
        {
            residue *other = dynamic_cast<residue *> (*iter);
            blackHole *bHole = dynamic_cast<blackHole *>(*iter);
            Block *block = dynamic_cast<Block *>(*iter);

            //If the residues collide with a block.
            if (block)
            {
                if( (x + radius  > block->getXPos() && x  + radius < block->getXPos() + block->getWidth())  && (y < block->getYPos() || y > block->getYPos() + block->getHeight()) )
                    yComponent = -yComponent;
                else
                    xComponent = -xComponent;
            }
            else if(bHole)      // if approaching black hole
            {
                //this->setZValue(bHole->zValue()-100);
                if(this->angle >=  M_PI)
                {
                    delete this;
                    return;
                }
                qreal r = bHole->getRadius();
                qreal b_x = bHole->getXPos(),
                        b_y = bHole->getYPos();

                setPos(b_x + cos(this->angle) * r,
                       b_y + sin(this->angle) * r);
                this->angle += 0.1;
                return;
            }
            else if(other) //otherwise, they are colliding with one another
            {
                qreal otherX = other->pos().x();
                qreal otherY = other->pos().y();
                //Make vectors representing our velocity and the item's velocity
                QVector2D othervec = QVector2D(otherX, otherY);
                QVector2D collision = resVec - othervec;
                QVector2D othervelocity = QVector2D(other->getXVel(), other->getYVel());
                qreal distance = collision.length();

                //if the ball is inside another object due to going too fast..
                if (distance  <= this->radius || distance <= this->radius)
                {
                    collision = QVector2D(1,0); // make sure not to divide by 0
                    distance = 1 ; //hack to make sure ball doesn't get stuck inside things
                }

                //calculate the components of the velocity vectors paralell to the collision
                collision = collision / distance; // normalise the collision vector
                qreal resCollisionVelocityInitial = QVector2D::dotProduct(resVelocity, collision);

                qreal otherCollisionVelocityInitial = QVector2D::dotProduct(othervelocity, collision);

                //assume the objects collide elastically, so the new velocities are given by...
                qreal resCollisionVelocityFinal = otherCollisionVelocityInitial;
                qreal otherCollisionVelocityFinal = resCollisionVelocityInitial;

                QVector2D newVelVec = collision *(resCollisionVelocityFinal - resCollisionVelocityInitial);
                this->setXVel(this->getXVel() + newVelVec.x());
                this->setYVel(this->getYVel() + newVelVec.y());

                QVector2D newOtherVelVec = collision * (otherCollisionVelocityFinal - otherCollisionVelocityInitial);
                other->setXVel(other->getXVel() + newOtherVelVec.x());
                other->setYVel(other->getYVel() + newOtherVelVec.y());
            }
        }
    }

    setPos(this->pos().x() + this->getXVel(), this->pos().y() + this->getYVel());

    //friction slow down
    qreal tFriction = t->getTableFriction();
    FrictionMovement(tFriction);
}

residue &residue::clone() const
{
    return *(new residue(*this));
}

bool residue::CheckInPocketRange(QList<pocket *> pl)
{
    qreal XDistance;
    qreal YDistance;
    qreal ballX = this->pos().x();
    qreal ballY = this->pos().y();
    //The use of the speed factor is to make the ball going into the pocket look more real
    //If the ball goes too fast, it would not get into the pocket
    qreal speedFactor = (qSqrt(qPow(xComponent,2)+qPow(yComponent,2)))/2;

    qreal distance;
    for(int i = 0; i<pl.size(); i++)
    {
        pocket * p = pl.at(i);
        XDistance = p->getXPos() - ballX;
        YDistance = p->getYPos() - ballY;

        //get the current speed of the ball
        qreal speed = qSqrt(qPow(xComponent,2)+qPow(yComponent,2));
        //qDebug()<<speed;

        distance = qSqrt(qPow(XDistance,2)+qPow(YDistance,2));


        //check if the ball is in the pocket range
        if(distance<=(p->getRadius()- speedFactor))
        {
            //check if the ball's speed is too fast(more than 25)
            //if it is, it passes by the pocket and both x and y velocity becomes 80% of the original velocities due to the pocket friction
            if(speed > 25)
            {
                xComponent = xComponent*0.8;
                yComponent = yComponent*0.8;
            }
            //otherwise, the ball falls into in pocket
            else
            {
                xComponent = 0;
                yComponent = 0;
                return true;
            }
        }

    }
    return false;

}
