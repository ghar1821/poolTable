#include "dialog.h"
#include "ui_dialog.h"


using namespace std;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    srand((unsigned)time(0));
    readFile(scene);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), scene, SLOT(advance()));


    timer->start(20);

}


Dialog::~Dialog()
{
    delete ui;
}

void Dialog::readFile(QGraphicsScene *scene)
{
    //table values
    double tableWidth;
    double tableHeight;
    double tableFriction;
    double cushionFriction;
    table *t;

    //ball values
    double ballPosX;
    double ballPosY;
    double ballVectorX;
    double ballVectorY;
    double ballRadius;

    //block values
    double blockX;
    double blockY;
    double blockWidth;
    double blockHeight;

    //pocket dimensions
    double pocketX;
    double pocketY;
    double pocketR;

    //blackhole values
    double bh_X, bh_Y, bh_power;

    double explosionSpeed;

    //A list of all items on the table. Includes balls, blocks, and pockets
    QList<TableItem *> items;

    //lists and a pointer to keep track if which item we're up to when reading input
    QList<pocket *> pocketL;
    pocket* p;

    QList<ball *> balls;
    ball * b;

    Block * block ;
    QList<Block *> blocks;

    blackHole *bh;
    QList<blackHole *>bHoles;


    //factory to create any balls read in
    BallFactory factory;

    ifstream myFile;
    //The config.txt file must be in the same folder with the executable file
    //Or the absolute path is needed
    std::string configFile = __FILE__;
    configFile.replace(configFile.end()-10,configFile.end(),"config.txt");
    myFile.open(configFile.c_str(), ifstream::in);
    if(!myFile.is_open())
    {
        cout<<"Could not open the file"<<endl;
        cout<<"An absolute path of the file is probably needed"<<endl;
        cout<< configFile;
        exit(1);
    }

    string value = "";
    while(!myFile.eof())
    {
        getline(myFile,value);

        //trim whitespace in front of the line
        value.erase(0,value.find_first_not_of(" \t\r\n"));

        //skip empty lines
        if(value.empty())
            continue;
        //skip commentary
        if(value.at(0)=='#')
            continue;

        if(value.find('=')==string::npos)
        {
            cout<<"Incorrect formatting or useless lines"<<endl;
            continue;
        }
        //Get the object and its corresponding values
        size_t sepIndex = value.find('=');
        string key = value.substr(0, sepIndex);
        string valueLine = value.substr(sepIndex+1);

        //Check what the exact object type the key is
        //And check if the format for these values are correct. Ignore the values writing after all correct values
        if(key=="Table")
        {
            istringstream line(valueLine);

            if(!(line >> tableWidth >> tableHeight >> tableFriction >> cushionFriction))
            {
                QMessageBox msg;
                msg.setText("Invalid format of or missing table values in configuration file. Please reset them.");
                msg.exec();
                exit(1);
            }
        }

        if(key=="Ball")
        {
            istringstream line(valueLine);

            if(!(line >> ballPosX >> ballPosY >> ballRadius >> ballVectorX >> ballVectorY))
            {
                QMessageBox msg;
                msg.setText("Invalid format of or missing ball values in configuration file. Please reset them.");
                msg.exec();
                exit(1);

            }
            //This ball has no table! Add it later when all pockets have been read in so the table can be made.

            b = factory.get( ballPosX, ballPosY, ballRadius, ballVectorX, ballVectorY);
            balls.append(b);

        }
        if(key=="Block")
        {
            istringstream line(valueLine);

            if(!(line >> blockX>> blockY>> blockWidth>> blockHeight))
            {
                QMessageBox msg;
                msg.setText("Invalid format of or missing block values in configuration file. Please reset them.");
                msg.exec();
                exit(1);

            }
            block = new Block(blockX, blockY, blockWidth, blockHeight);
            blocks.append(block);

        }

        if(key=="Pocket")
        {
            istringstream line(valueLine);

            if(!(line >> pocketX >> pocketY >> pocketR))
            {
                QMessageBox msg;
                msg.setText("Invalid format of or missing pocket values in configuration file. Please reset them.");
                msg.exec();
                exit(1);
            }

            p = new pocket(pocketX, pocketY, pocketR, 0,0);
            pocketL.append(p);
        }

        if(key=="BlackHole")
        {
            istringstream line(valueLine);

            if(!(line >> bh_X >> bh_Y >> bh_power))
            {
                QMessageBox msg;
                msg.setText("Invalid format of or missing pocket values in configuration file. Please reset them.");
                msg.exec();
                exit(1);
            }
            bh = new blackHole(bh_X,bh_Y, 10, bh_power);
            bHoles.append(bh);
        }

        if(key=="Explosion")
        {
            istringstream line(valueLine);
            if(!(line >> explosionSpeed))
            {
                QMessageBox msg;
                msg.setText("Invalid format of or missing pocket values in configuration file. Please reset them.");
                msg.exec();
                exit(1);
            }
        }

    }
    cout<<"Finished reading the file."<<endl;
    myFile.close();
    cout<<"File closed."<<endl;

    for(int i = 0; i<pocketL.size();i++)
    {
        pocket* p1 = pocketL.at(i);
        if( p1==0)
            continue;
        if(p1->getRadius()<=ballRadius)
        {
            QMessageBox msg;
            msg.setText("The pocket must be bigger than the ball.");
            msg.exec();
            exit(1);
        }
    }
    // check that all blocks are within the table
    // These are newly added because the given method doesn't work as the method
    // validPosition for tableItem checks the position of the object using radius,
    // and block has no radius and the previous programmer do not initialise the radius
    // of the block and take some crazy arbitrary values that causes run time error.
    for(QList<Block *>::iterator iter = blocks.begin();
        iter != blocks.end();
        ++iter)
    {
        qreal x = (*iter)->getXPos();
        qreal y = (*iter)->getYPos();
        if(x > tableWidth || x < 0 || y > tableHeight || y < 0)
        {
            blocks.erase(iter);
        }
        items.append(*iter);
    }

    // check all pockets are within the table
    for(QList<pocket *>::iterator iter = pocketL.begin();
        iter != pocketL.end();
        ++iter)
    {
        if( !(*iter)->validPosition(tableWidth,tableHeight) )
            pocketL.erase(iter);
        items.append(*iter);

    }

    qreal largestBallRadius = 10;       // to create blackhole fitting all the balls
    // check all the balls are within the table
    for(QList<ball *>::iterator iter = balls.begin();
        iter != balls.end();
        ++iter)
    {
        if( !(*iter)->validPosition(tableWidth,tableHeight) )
            balls.erase(iter);
        if((*iter)->getRadius() > largestBallRadius)
            largestBallRadius = (*iter)->getRadius();
        items.append(*iter);

    }

    // Adding and checking blackhole's position
    for(QList<blackHole *>::iterator iter = bHoles.begin();
        iter != bHoles.end();
        ++iter)
    {
        (*iter)->setRadius(largestBallRadius);
        if( !(*iter)->validPosition(tableWidth, tableHeight) )
            bHoles.erase(iter);
        items.append(*iter);
    }
    bool anyOverlap = false;    // used to show message box if there are overlapping items
    //check that all items don't overlap. Balls, blocks, pockets are all included
    for(QList<TableItem *>::iterator iter = items.begin(); iter != items.end(); ++iter)
    {
        //if the current item overlaps with any other item
        if (overlapping(*iter, &items))
        {
            anyOverlap = true;
            bool isBlock = dynamic_cast<Block *>(*iter) != NULL;
            bool isBlackHole = dynamic_cast<blackHole *>(*iter) != NULL;
            bool isPocket = dynamic_cast <pocket *> (*iter) != NULL;
            bool isBall= dynamic_cast <ball *> (*iter) != NULL;

            // remove items from specific list
            if (isPocket)
                pocketL.removeOne(dynamic_cast<pocket *> (*iter));
            if (isBall)
                balls.removeOne(dynamic_cast<ball *> (*iter));
            if (isBlock)
                blocks.removeOne(dynamic_cast<Block *> (*iter));
            if (isBlackHole)
                bHoles.removeOne(dynamic_cast<blackHole *> (*iter));

            items.erase(iter); //erase the current overlapping one and let the new one stay
        }
    }
    if(anyOverlap)
    {
        QMessageBox msg;
        msg.setText("Some items are overlapping!\nSome of them are discarded!");
        msg.exec();
    }

    //Pen for table
    QColor edgeColor = QColor(100,70,50,255);
    QPen pen = QPen(edgeColor);
    double penWidth = 10;
    pen.setWidth(penWidth);

    // create table and scene
    t = new table(tableWidth, tableHeight, penWidth, tableFriction, cushionFriction, pocketL);

    // modify the balls' explosion speed if it's not valid
    if(explosionSpeed < 0)
        explosionSpeed *= -1;

    // add the table to all the balls
    // and set the speed that will cause the ball to explode
    for(QList<ball *>::iterator iter = balls.begin(); iter != balls.end(); ++iter)
    {
        (*iter)->addTable(t);
        (*iter)->setExplosionSpeed(explosionSpeed);
    }

    //check that all the pockets are valid
    checkPocketPos(t, pocketL);

    //inappropriate friction
    if(cushionFriction >= 1 || tableFriction >= 1 || cushionFriction<=0 || tableFriction<=0)
    {
        QMessageBox msg;
        msg.setText("The range of friction should be (0,1). Please reset the friction values.");
        msg.exec();
        exit(1);
    }

    //setFixedSize(tableWidth+(penWidth*2), tableHeight+(2*penWidth));
    scene->setSceneRect(0,0,tableWidth, tableHeight);
    ui->graphicsView->setFixedWidth(tableWidth + penWidth * 2);
    ui->graphicsView->setFixedHeight(tableHeight + penWidth * 2);
    //Add the items to the scene
    for(QList<TableItem *>::iterator iter = items.begin(); iter != items.end(); ++iter)
    {
        scene->addItem(*iter);
    }

    //Create table in scene
    QLineF TopEdge(scene->sceneRect().topLeft(), scene->sceneRect().topRight());
    QLineF LeftEdge(scene->sceneRect().topLeft(), scene->sceneRect().bottomLeft());
    QLineF RightEdge(scene->sceneRect().topRight(), scene->sceneRect().bottomRight());
    QLineF BottomEdge(scene->sceneRect().bottomLeft(), scene->sceneRect().bottomRight());

    scene->addLine(TopEdge, pen);
    scene->addLine(LeftEdge, pen);
    scene->addLine(RightEdge, pen);
    scene->addLine(BottomEdge, pen);

    QColor tColor = QColor(70,140,50,255);
    scene->setBackgroundBrush(tColor);
}

//returns true if the item overlaps any of the others
bool Dialog::overlapping(TableItem * item, QList<TableItem * > * others)
{

    for (QList<TableItem *>::iterator iter = others->begin(); iter != others->end(); ++iter)
    {

        TableItem * other = *iter;
        //Of course we overlap with ourself, so don't check that.
        if (other == item)
            continue;

        //check if overlapping

        if(item->collidesWithItem(other))
            return true;        
    }
    return false;
}


void Dialog::checkPocketPos(table *t, QList<pocket *> pl)
{
    qreal width = t->getWidth();
    qreal height = t->getHeight();
    qreal penwidth = t->getPenWidth();

    qreal xLeft = penwidth/2;
    qreal xRight = width - penwidth/2;
    qreal yUp = penwidth/2;
    qreal yBottom = height - penwidth/2;
    for(int i = 0; i<pl.size(); i++)
    {
        if(!PocketInRange(pl.at(i),xLeft, xRight, yUp, yBottom))
        {
            QMessageBox msg;
            msg.setText("The pocket is not in range. Please reset the position of the pocket.");
            msg.exec();
            exit(1);
        }
    }
}

bool Dialog::PocketInRange(pocket *p,qreal xLeft, qreal xRight, qreal yUp, qreal yBottom)
{
    qreal radius = p->getRadius();
    qreal x = p->getXPos();
    qreal y = p->getYPos();

    if((x-radius)<xLeft || (x+radius)>xRight || (y-radius)<yUp ||(y+radius)>yBottom)
        return false;

    return true;



}


