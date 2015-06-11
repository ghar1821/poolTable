#include "ball.h"

using namespace std;
ball::ball(qreal posX, qreal posY, qreal r, qreal VX, qreal VY, table *table1):
    TableItem(posX, posY, r, VX, VY),
    t(table1)
{   setPos(startX, startY);
    this->angle = 0;                    // the circular movement of the ball. The increment is determined by the suction power of the black hole.
    this->playBlackHoleSound = true;    // only play the sound once when the ball is sucked into the black hole.
    this->explosionSpeed = 10;          // default value in case the speed is not given in the config file
}

ball::~ball()
{}

QRectF ball::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(-radius - penWidth / 2, -radius - penWidth / 2,
                      radius*2 + penWidth, radius*2 + penWidth);
}

ball & ball::clone() const
{return  *(new ball(*this));}

void ball::explode()
{
    yComponent = 0;
    xComponent = 0;
    chomp->play(":/sounds/crunch.wav");
    int dirX = 0, dirY = 0;
    qreal x = pos().x();
    qreal y = pos().y();

    // Randomise the direction of residues heading
    dirX = rand() % 2;
    dirY = rand() % 2;
    if(dirX == 0) dirX = -1;
    if(dirY == 0) dirY = -1;

    residue *res = new residue(x, y, 0.50, dirX*(1+rand()%5), dirY*(1+rand()%5), this->getTable() );
    QList<residue *> residues;
    residues.append(res);

    // number of residue is determined by the config file + 10.
    // the explosionSpeed variable is read in from the config file.
    for(int i = 0; i != this->explosionSpeed+10; ++i)
    {
        // Randomise the direction of residues heading
        dirX = rand() % 2;
        dirY = rand() % 2;
        if(dirX == 0) dirX = -1;
        if(dirY == 0) dirY = -1;

        // Randomise the position of the residue, but make it within the area of the ball.
        qreal t_X, t_Y;
        t_X = x + (rand()%((int)this->radius*2));
        t_Y = y + rand()%((int)this->radius*2);

        // make sure the residues are not overlapping one another.
        for(QList<residue *>::iterator iter = residues.begin(); iter != residues.end(); ++iter)
        {
            while(t_X == (*iter)->pos().x())
                t_X = x + rand()%((int)this->radius*2);
            while(t_Y == (*iter)->pos().y())
                t_Y = y + rand()%((int)this->radius*2);
        }
        residues.append(new residue(t_X,t_Y, 0.50, dirX*(1+rand()%5), dirY*(1+rand()%5),this->getTable()));
    }
    // Add the residues into the table
    for(QList<residue *>::iterator iter = residues.begin(); iter!=residues.end();++iter)
    {
        (*iter)->setZValue(-1);
        scene()->addItem(*iter);
    }

    // remove the ball from the scene
    scene()->removeItem(this);

}

void ball::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(!CheckInPocketRange(t->getPocketList()))
    {
        painter->setBrush(Qt::white);
        painter->drawEllipse(boundingRect());
    }
    //if the ball object is in the pocket, delete this ball object
    else
    {
        delete this;
        qDebug()<<"A ball object in the pocket has been deleted";
    }

}

void ball::advance(int phase)
{
    if(!phase) return;

    QPointF next_point=mapToParent(xComponent,yComponent);
    qreal next_x = next_point.x();
    qreal next_y = next_point.y();
    qreal new_x = next_x;
    qreal new_y = next_y;

    qreal width = t->getWidth();
    qreal height = t->getHeight();

    qreal penWidth = t->getPenWidth();

    qreal widthBound = width-radius-penWidth/2;
    qreal heightBound = height-radius-penWidth/2;


    // Checking if the ball hit the boundary of the table
    while(XOffBoundary(t->getWidth(), new_x) || YOffBoundary(t->getHeight(), new_y))
    {
        qreal speed = qSqrt(qPow(xComponent,2)+qPow(yComponent,2));
        if(speed > this->explosionSpeed)
        {
            this->explode();
            delete this;
            return;
        }
        chomp->play(":/sounds/spring.wav");
        //right boundary
        if(next_x >= widthBound){
            new_x = 2*widthBound - next_x;
            xComponent= -xComponent;
        }
        //left boundary
        if(next_x <= (radius+penWidth/2)){
            new_x = 2*(radius+penWidth/2)-next_x;
            xComponent= -xComponent;
        }
        //bottom boundary
        if(next_y >= heightBound){
            new_y = 2*heightBound - next_y;
            yComponent= -yComponent;
        }
        //top boundary
        if(next_y <= (radius+penWidth/2)){
            new_y = 2*(radius+penWidth/2)-next_y;
            yComponent= -yComponent;
        }

        qreal cFriction = t->getCushionFriction();
        CushionEnergyLoss(cFriction);
    }


    //Now, check if the ball is colliding with an item other than the table

    //If we are colliding with something
    QList<QGraphicsItem *> colliders = collidingItems();
    if(colliders.size() != 0)
    {
        // if the collision speed is higher than the threshold set in the config file
        // explode!
        qreal speed = qSqrt(qPow(xComponent,2)+qPow(yComponent,2));
        if(speed > this->explosionSpeed)
        {
            this->explode();
            delete this;
            return;
        }

        // otherwise check if the ball is colliding with some objects on the table
        else
        {
            qreal x = pos().x();
            qreal y = pos().y();
            QVector2D ballvec = QVector2D(x, y);
            QVector2D ballvelocity = QVector2D(this->getXVel(), this->getYVel());

            //For every item that we collide with
            for(QList<QGraphicsItem *>::iterator iter = colliders.begin(); iter != colliders.end(); ++iter)
            {
                ball * other = dynamic_cast<ball *> (*iter);
                Block *block = dynamic_cast<Block *>(*iter);
                residue *r = dynamic_cast<residue *> (*iter);
                blackHole *bHole = dynamic_cast<blackHole *>(*iter);


                if(r)   // if roll over residue
                {
                    qreal tFriction = t->getTableFriction();
                    FrictionMovement(tFriction);
                }

                //If we didn't collide with a ball, see if we collided with a block. Collisions must be handled differently since blocks are fixed objects, and balls can move.
                if (block)
                {
                    chomp->play(":/sounds/spring.wav");
                    //if we're colliding vertically, switch the ball's y component
                    if( (x + radius  > block->getXPos() && x  + radius < block->getXPos() + block->getWidth())  && (y < block->getYPos() || y > block->getYPos() + block->getHeight()) )
                        yComponent = -yComponent;
                    else
                        //otherwise we're colliding horizontally, and reverse the y component
                        xComponent = -xComponent;
                }
                else if(bHole)      // if approaching black hole
                {
                    // if the ball has rotated for quite a while, just delete the ball.
                    if(this->angle >=  M_PI)
                    {
                        delete this;
                        return;
                    }
                    if(playBlackHoleSound)
                    {
                        chomp->play(":/sounds/whip.wav");
                        this->playBlackHoleSound = false;
                    }

                    qreal r = bHole->getRadius();

                    qreal b_x = bHole->getXPos(),
                            b_y = bHole->getYPos();

                    // The ball moves in circular motion
                    setPos(b_x + cos(this->angle) * r,
                           b_y + sin(this->angle) * r);
                    this->angle += bHole->returnPower();
                    return;
                }

                else if(other) //otherwise, we're colliding with a ball
                {
                    chomp->play(":/sounds/clang.wav");
                    qreal otherX = other->pos().x();
                    qreal otherY = other->pos().y();
                    //Make vectors representing our velocity and the item's velocity
                    QVector2D othervec = QVector2D(otherX, otherY);
                    QVector2D collision = ballvec - othervec;
                    QVector2D othervelocity = QVector2D(other->getXVel(), other->getYVel());
                    qreal distance = collision.length();

                    //if the ball is inside another object due to going too fast..
                    if (distance  <= this->getRadius())
                    {
                        qDebug()<< "inside another object!" << other;
                        collision = QVector2D(1,0); // make sure not to divide by 0
                        distance = 1 ; //hack to make sure ball doesn't get stuck inside things
                    }

                    //calculate the components of the velocity vectors paralell to the collision
                    collision = collision / distance; // normalise the collision vector
                    qreal ballCollisionVelocityInitial = QVector2D::dotProduct(ballvelocity, collision);

                    qreal otherCollisionVelocityInitial = QVector2D::dotProduct(othervelocity, collision);

                    //assume the objects collide elastically, so the new velocities are given by...
                    qreal ballCollisionVelocityFinal = otherCollisionVelocityInitial;
                    qreal otherCollisionVelocityFinal = ballCollisionVelocityInitial;

                    QVector2D newVelVec = collision *(ballCollisionVelocityFinal - ballCollisionVelocityInitial);
                    this->setXVel(this->getXVel() + newVelVec.x());
                    this->setYVel(this->getYVel() + newVelVec.y());

                    QVector2D newOtherVelVec = collision * (otherCollisionVelocityFinal - otherCollisionVelocityInitial);
                    other->setXVel(other->getXVel() + newOtherVelVec.x());
                    other->setYVel(other->getYVel() + newOtherVelVec.y());
                }

            }
        }
    }


    setPos(this->pos().x() + this->getXVel(), this->pos().y()+ this->getYVel());

    //friction slow down
    qreal tFriction = t->getTableFriction();
    FrictionMovement(tFriction);

    //get the current speed of the ball
    qreal speed = qSqrt(qPow(xComponent,2)+qPow(yComponent,2));

    if(speed<0.1)
    {
        xComponent = 0;
        yComponent = 0;
    }

}
bool ball::CheckInPocketRange(QList<pocket *> pl)
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

void ball::addTable(table *newTable)
{this->t = newTable;}

void ball::setExplosionSpeed(double spd)
{
    this->explosionSpeed = spd;
}

table *ball::getTable()
{return t;}
