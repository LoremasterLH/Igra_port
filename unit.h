#ifndef UNIT_H
#define UNIT_H

#include "colors.h"
#include <QPoint>
#include <QBitmap>
#include <QGraphicsItem>


class Unit : public QPolygonF // : QGraphicsItem //class for managing objects on board
{
private:
    qint8 race; //0 = black, 1 = green, 2 = red, 3 = white
    QString name;
    int price;
    QString description;
    int movesLeft; //moves avaible this turn
    int maxMoves; //max moves
    int attack;
    int hp;
    int maxHp;
    int armor;
    int range; //0 = melee
    int type; //0 = fort, 1 = infantry, 2 = archer, 3 = cavalry, 4 = siege
    QPixmap image; //path to image
    QPoint location; //zgornji levi kot

public:
    Unit();
    Unit(qint8 Race, QString Name, int Price, QString Description, int MovesLeft, int Attack, int Hp, int Armor, int Range, int Type, QPixmap Image, QPoint Location=QPoint(-1,-1)); //starting creation
    Unit(Unit unit, QPoint Location);

    void setMovesLeft(int moves);
    void setLocation(QPoint loc);
    void setDamageTaken(int damage); //possible change for healing; just insert a negative value and check if it doesn't overflow maxHp

    qint8 getRace();
    QString getRaceString();
    QString getName();
    int getPrice();
    QString getDescription();
    int getMovesLeft();
    int getMaxMoves();
    int getAttack();
    int getHp();
    int getArmor();
    int getRange();
    int getType();
    QPixmap getImage();
    QPoint getLocation();
};

#endif // UNIT_H
