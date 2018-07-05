#include "board.h"
#include <QDebug>
#include <QMouseEvent>

Board::Board()
{
    scene = new QGraphicsScene(0,0,board_width,board_height,this);
    scene->addRect(0,0,board_width,board_height,QPen(Qt::green)); //field border

    for(int i=0; i<w;i++)//scene->width(); i++)
        {
            for(int j=0; j<h; j++)
            {
                fields[i][j] = addHexagon(indexToBoard(i,j)); //same as below, just vertically it is an aditional y down
                scene->addPolygon(fields[i][j],QPen(),QBrush(QColor(245,222,100))); //wheat color
            }
        }

    selected = -1;
    validHexes = NULL;
}

void Board::setCurrentPlayer(qint8 player)
{
    currentPlayer = player;
}

void Board::mousePressEvent(QMouseEvent* event)
{
    //removeUnit(units[0]);
    //qDebug() << "Mouse moved!";

    for(int i=0; i<w; i++)
        for(int j=0; j<h; j++)
            if(fields[i][j].containsPoint(QPointF(event->x(),event->y()),Qt::WindingFill))
            {
                //qDebug() << "Hex found! Initiate boarding procedures! Hex ID: " << i << j;
                //QBrush brush = QBrush(Qt::red);
                QPen pen = QPen(Qt::cyan);


                //scene->addPolygon(fields[i][j],pen);

                //if(scene->items())
                //scene->itemAt(i,j);
                if(Qt::LeftButton == event->button()) //select
                {
                    qDebug() << "Left click";
                    pen.setColor(Qt::cyan);

                    if(validHexes) //if there are colored hexes around
                    {
                        for(int l=0; l<validHexes->count();l++) //clear selection before moving unit, because it is on top
                            scene->removeItem(scene->itemAt(validHexes->at(l).at(1).x(),validHexes->at(l).at(0).y(),QTransform())); //this point is located inside the hexagon half a distance from center
                        delete validHexes;
                        validHexes = NULL;
                    }
                    //selected_field = Unit();
                    for(int k=activeUnits.count()-1; k>=0; k--) //find clicked unit
                    {
                        //qDebug() << units[k].getLocation() << QPoint(i,j);
                        if(activeUnits[k].getRace() == currentPlayer && activeUnits[k].getLocation() == QPoint(i,j))
                        {
                            qDebug() <<activeUnits[k].getLocation() << QPoint(i,j);

                            //scene->addPolygon(fields[i][j],pen,brush);
                            selectHex(pen,i,j);

                            if(selected != -1) //previous selection exists
                            {
                                qDebug() << "Selection location: " << selected << activeUnits[selected].getLocation();
                                scene->removeItem(scene->itemAt(indexToBoard(activeUnits[selected].getLocation().x(),activeUnits[selected].getLocation().y()),QTransform()));
                            }

                            /*if(selected_field.getRace()!=-1) //if previous selection exists clears it
                            {
                                qDebug() << "Remove selection";
                                //qDebug() << "on entry " << selected_field.getRace();
                                //removeUnit(selected_field);
                                //scene->removeItem(scene->itemAt(selected_field.getLocation(),QTransform()));
                                //for(int l=0; l<selection->count();l++)
                                    //scene->removeItem(scene->itemAt(selection->at(l).at(1).x(),selection->at(l).at(0).y(),QTransform())); //this point is located inside the hexagon half a distance from center
                                //delete selection;
                                removeObject(selected_field);
                                selected_field = Unit();
                                //qDebug() << "on exit " << selected_field.getRace();
                                //qDebug() << units.count() << " > " << k;
                            }*/

                            validHexes = new QList<QPolygonF>(hexesInRange(activeUnits[k].getMovesLeft(),QPoint(i,j)));
                            selected = k;


                            emit showUnitDescription(activeUnits[selected]);
                            //selected_field = activeUnits[k];
                            //unitID = k;
                            qDebug() << "Moves: " << activeUnits[selected].getMovesLeft();
                            //if(!selection)

                            //selection = new QList<QPolygonF>(hexesInRange(2,QPoint(i,j)));
                            qDebug() << "Selection: " << validHexes->count();

                            for(int l=0; l<validHexes->count();l++)
                            {
                                //qDebug() <<"selection";
                                scene->addPolygon(validHexes->at(l),QPen(Qt::magenta));
                            }
                                //selectHex(QPen(Qt::black),selected_field.getLocation().x(),selected_field.getLocation().y());
                            break; //only interested in first unit, if there are more
                        }
                    }
                }
                else //attack or move
                {
                    qDebug() << "Right click";
                    pen.setColor(Qt::black);

                    //if(selected_field.getRace() == -1)
                    {
                        //scene->addPolygon(fields[selected_field.getLocation().x()][selected_field.getLocation().y()]);
                        //selectHex(pen,selected_field.getLocation().x(),selected_field.getLocation().y());


                        bool delete_selection = false;
                        //qDebug() << "Selected unit: " << selected;
                        if(selected > 0 && activeUnits[selected].getMovesLeft()>0)
                        {
                            //qDebug() << "Selection count" << validHexes->count();
                            //qDebug() << "Moves left: " << activeUnits[selected].getMovesLeft();
                            for(int l=0; l<validHexes->count();l++)
                            {
                                //qDebug() << coloredHexes->at(l).x() << i << coloredHexes->at(l).y() << j;
                                if(coloredHexes->at(l).x()==i && coloredHexes->at(l).y()==j)
                                {
                                    //qDebug() << "Moving unit";
                                    for(int l=0; l<validHexes->count();l++) //clear selection before moving unit, because it is on top
                                        scene->removeItem(scene->itemAt(validHexes->at(l).at(1).x(),validHexes->at(l).at(0).y(),QTransform())); //this point is located inside the hexagon half a distance from center

                                    bool attack_move=false;
                                    for(int l=0;l<activeUnits.count();l++) //first placed unit attacked first, in case of stacked units (Fortress)
                                    {
                                        if(activeUnits[l].getLocation() == QPoint(i,j))
                                        {
                                            //qDebug() << "Location of target: " << activeUnits[l].getLocation();
                                            attack_move=true;
                                            qDebug() << "Attack issued!";
                                            if(activeUnits[l].getRace() != currentPlayer)
                                            {
                                                int damage=attackUnit(activeUnits[selected],activeUnits[l]);
                                                if(damage!=-1000) //yeh...
                                                    activeUnits[l].setDamageTaken(damage);
                                                activeUnits[selected].setMovesLeft(activeUnits[selected].getMovesLeft()-1); //decrease moves
                                            }
                                            else
                                            {
                                                removeObject(activeUnits[selected]); //removes selection
                                                appendOutputText(tr("We won't attack our own troops!"));
                                            }
                                            break;
                                        }
                                    }
                                    if(!attack_move)
                                    {
                                        qDebug() << "Move issued";
                                        moveUnit(activeUnits[selected],QPoint(i,j)); //move unit, if nothing to attack at target spot
                                        activeUnits[selected].setMovesLeft(activeUnits[selected].getMovesLeft()-1); //decrease moves
                                    }

                                    //qDebug() << activeUnits.count() << selected+1;

                                    delete_selection = true;
                                    selected = -1;
                                    break;
                                }
                                //scene->removeItem(scene->itemAt(selection->at(l).at(1).x(),selection->at(l).at(0).y(),QTransform())); //this point is located inside the hexagon half a distance from center
                            }
                        }
                        else
                            emit appendOutputText(tr("No moves left on the unit! It can not move until next turn."));


                        if(delete_selection && validHexes)
                        {
                            delete validHexes;
                            validHexes = NULL;
                        }

                        /*removeObject(selected_field);
                        removeObject(selected_field,true);
                        addUnit(selected_field,QPoint(i,j));*/
                        //scene->removeItem(scene->itemAt(selected_field.getLocation(),QTransform()));
                        //removeUnit(selected_field);
                        //removeUnit(selected_field);
                        //emit sendUpdateBoard();
                        //addUnit(selected_field,QPoint(i,j));
                        //selected_field = Unit();
                        //qDebug() << "Race " << activeUnits[selected].getRace();
                    }

                }

                //scene->addPolygon(fields[i][j],pen,brush);

                /*foreach(Unit unit,units)
                {
                    if()
                }*/


                //emit sendUpdateBoard();
            }
}

/*void Board::mouseReleaseEvent(QMouseEvent* event)
{
    //qDebug() << event;
}*/

QList<QPolygonF> Board::hexesInRange(int range, QPoint loc) //moves or ranged (separate calculations)
{   
    QList<QPolygonF> hexes;

    if(0 == range)
        return hexes;

    coloredHexes = new QList<QPoint>();

    //qDebug() << "Range: " << range;


    if(loc.y()!=0)
        hexes.append(moveUp(loc));

    //if(loc.x()%2!=0 && loc.y()!=0 || loc.x()!=w-1)
    if(loc.x()!=w-1 && !(loc.y()==0 && loc.x()%2==0))
        hexes.append(moveRightUp(loc));

    //if(loc.x()%2!=0 && loc.y()!=h-1 || loc.x()!=w-1)
    if(loc.x()!=w-1 && !(loc.y()==h-1 && loc.x()%2!=0))
        hexes.append(moveRightDown(loc));

    if(loc.y()!=h-1)
        hexes.append(moveDown(loc));

    //if(loc.x()%2==0 && (loc.x()!=0 || loc.y()!=h-1))
    if(loc.x()!=0 && !(loc.y()==h-1 && loc.x()%2!=0))
        hexes.append(moveLeftDown(loc));

    //if(loc.y()!=0 || loc.x()%2!=0)
    if(loc.x()!=0 && !(loc.y()==0 && loc.x()%2==0))
        hexes.append(moveLeftUp(loc));

    /*while(range > 0)
    {
        qDebug() << range;
        for(int i=0;i<coloredHexes->count();i++)
            hexes.append(hexesInRange(range-1,coloredHexes->at(i)));
        range--;
    }*/

    //delete coloredHexes;
    return hexes;
}

QPolygonF Board::moveUp(QPoint loc)
{
    coloredHexes->append(QPoint(loc.x(),loc.y()-1));
    return fields[loc.x()][loc.y()-1];
}

QPolygonF Board::moveDown(QPoint loc)
{
    coloredHexes->append(QPoint(loc.x(),loc.y()+1));
    return fields[loc.x()][loc.y()+1];
}

QPolygonF Board::moveLeftUp(QPoint loc)
{
    if(loc.x()%2!=0)
    {
        coloredHexes->append(QPoint(loc.x()-1,loc.y()));
        return fields[loc.x()-1][loc.y()];
    }
    else
    {
        coloredHexes->append(QPoint(loc.x()-1,loc.y()-1));
        return fields[loc.x()-1][loc.y()-1];
    }
}

QPolygonF Board::moveLeftDown(QPoint loc)
{
    if(loc.x()%2!=0)
    {
        coloredHexes->append(QPoint(loc.x()-1,loc.y()+1));
        return fields[loc.x()-1][loc.y()+1];
    }
    else
    {
        coloredHexes->append(QPoint(loc.x()-1,loc.y()));
        return fields[loc.x()-1][loc.y()];
    }
}

QPolygonF Board::moveRightUp(QPoint loc)
{
    if(loc.x()%2!=0)
    {
        coloredHexes->append(QPoint(loc.x()+1,loc.y()));
        return fields[loc.x()+1][loc.y()];
    }
    else
    {
        coloredHexes->append(QPoint(loc.x()+1,loc.y()-1));
        return fields[loc.x()+1][loc.y()-1];
    }
}

QPolygonF Board::moveRightDown(QPoint loc)
{
    if(loc.x()%2!=0)
    {
        coloredHexes->append(QPoint(loc.x()+1,loc.y()+1));
        return fields[loc.x()+1][loc.y()+1];
    }
    else
    {
        coloredHexes->append(QPoint(loc.x()+1,loc.y()));
        return fields[loc.x()+1][loc.y()];
    }
}

void Board::moveUnit(Unit &unit, QPoint target)
{
    qDebug() << "Moving (moveUnit(...))";
    removeObject(unit,false,true); //remove selection
    removeObject(unit,true,true); //remove unit
    addUnit(unit,target,true);
    emit showUnitDescription(unit);
}

int Board::attackUnit(Unit &attacker, Unit &target) //add translator for this function!
{
    qDebug() << "Attacking (attackUnit(...))";
    //qDebug() << "Location of target: " << target.getLocation();
    QPoint target_loc = target.getLocation();
    appendOutputText(tr("%1 (%2) hits %3 (%4) for %5 damage (%6 absorbed by armor)").arg(attacker.getName(), attacker.getRaceString(),target.getName(),target.getRaceString(),QString::number(attacker.getAttack() - target.getArmor()),QString::number(target.getArmor())));
    if(attacker.getAttack() >= target.getArmor() + target.getHp())
    {
        appendOutputText(tr("%1 (%2) has been destroyed").arg(target.getName(), target.getRaceString()));
        removeObject(target,true);
        moveUnit(attacker,target_loc);
        //return ; //if fatal damage
    }
    else if(attacker.getAttack() > target.getArmor()) //if attacker can damage target
    {
        removeObject(attacker,false,true); //remove selection
        return attacker.getAttack() - target.getArmor();
    }
    else
        removeObject(attacker,false,true); //remove selection

    emit showUnitDescription(attacker);
    //qDebug() << "Attack succesfully executed!";
    return -1000; //prolly good to change to osme other value
}

int Board::clickedHexContainsUnit(Unit &unit)
{
    for(int i=activeUnits.count()-1; i>=0; i--) //last to first, since units can stack on fortress
    {
        if(activeUnits[i].getLocation()==unit.getLocation()) //removes first unit at the specified location from internal storage
        {
            return i;
        }
    }
    qDebug() << "No unit found!";
    return -1;
}

void Board::selectHex(QPen pen, int x, int y)
{
    scene->addPolygon(fields[x][y],pen);
}

/*void Board::unselectHex(int x, int y)
{

}*/

QPolygonF Board::addHexagon(QPointF center) //border length
{
    QPolygonF hexagon;
    QPointF first;
    first.setX(center.x()-a);
    first.setY(center.y());
    //qDebug(first.x() + " " + first.y());

    QPointF second;
    second.setX(first.x()+a*qCos(M_PI*300/180)); // 60/180 //cos(300) = cos(60) = 0,5
    second.setY(first.y()+a*qSin(M_PI*300/180));
    //qDebug(second.x() + " " + second.y());

    QPointF third;
    third.setX(second.x()+a);
    third.setY(second.y());
    //qDebug(third.x() + " " + third.y());

    QPointF fourth;
    fourth.setX(center.x()+a);
    fourth.setY(center.y());
    //qDebug(fourth.x() + " " + fourth.y());

    QPointF fifth;
    fifth.setX(third.x());
    fifth.setY(fourth.y()+a*qSin(M_PI*60/180)); //cos(60) = 0,5
    //qDebug(fifth.x() + " " + fifth.y());

    QPointF sixth;
    sixth.setX(fifth.x()-a);
    sixth.setY(fifth.y());
    //qDebug(sixth.x() + " " + sixth.y());

    return hexagon << first << second << third << fourth << fifth << sixth;
}

void Board::addUnit(Unit& unit, QPoint loc, bool is_move)
{
    qDebug() << "Adding (addUnit(...))";
    QBrush brush(unit.getImage().scaled(1.5*a,1.5*a));
    QGraphicsPolygonItem* item = new QGraphicsPolygonItem(fields[loc.x()][loc.y()]);

    item->setBrush(brush);
    scene->addItem(item);
    unit.setLocation(loc);
    if(!is_move)
    {
        activeUnits.append(unit);
        qDebug() << "Unit sucesfully added!";
    }
}

void Board::removeObject(Unit& unit, bool is_unit, bool is_move) //can lso remove
{
    qDebug() << "Removing (removeObject(...))";
    QPointF point = indexToBoard(unit.getLocation().x(),unit.getLocation().y());
    scene->removeItem(scene->itemAt(point,QTransform()));
    if(!is_move && is_unit)
    {
        activeUnits.removeAt(clickedHexContainsUnit(unit));
        qDebug() << "Unit sucesfully removed!";
    }
}

QSize Board::getSize()
{
    return QSize(w,h);
}

QPointF Board::indexToBoard(int i, int j)
{
    if(i%2)
        return QPointF(a+i*x,2*y+j*2*y); //same as below, just vertically it is an aditional y down
    else
        return QPointF(a+i*x,y+j*2*y); //center is for a right horizontally and for y down vertically
}
