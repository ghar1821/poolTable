#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QSettings>
#include <QVariant>

#include <QTextStream>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <list>
#include <QList>
#include <ctime>

#include "ballfactory.h"
#include "block.h"
#include "table.h"
#include "pocket.h"
#include "ball.h"
#include "blackhole.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void readFile(QGraphicsScene * scene);
    void checkPocketPos(table * t, QList<pocket *> pl);
    bool PocketInRange(pocket *p, qreal xLeft, qreal xRight, qreal yUp, qreal yBottom);
    QList<TableItem *> addMany(QList<TableItem *> * items, double ballRadius);
    bool overlapping(TableItem * item, QList<TableItem * > * others);


private:
    Ui::Dialog *ui;
    QGraphicsScene *scene;
    QTimer *timer;
};

#endif // DIALOG_H
