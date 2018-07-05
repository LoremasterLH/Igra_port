#include "unit.h"

Unit::Unit() //dud unit
{
    race = -1;
}

Unit::Unit(qint8 Race, QString Name, int Price, QString Description, int MovesLeft, int Attack, int Hp, int Armor, int Range, int Type, QPixmap Image, QPoint Location)
{
    race = Race;
    name = Name;
    price = Price;
    description = Description;
    maxMoves = movesLeft = MovesLeft;
    attack = Attack;
    maxHp = hp = Hp;
    armor = Armor;
    range = Range;
    type = Type;
    image = Image;
    location = Location;
}

Unit::Unit(Unit unit, QPoint Location)
{
    race = unit.getRace();
    name = unit.getName();
    price = unit.getPrice();
    description = unit.getDescription();
    movesLeft = unit.getMovesLeft();
    attack = unit.getAttack();
    hp = unit.getHp();
    armor = unit.getArmor();
    range = unit.getRange();
    image = unit.getImage();
    location = Location;
}

void Unit::setLocation(QPoint loc)
{
    location = loc;
}

void Unit::setMovesLeft(int moves)
{
    movesLeft = moves;
}

void Unit::setDamageTaken(int damage)
{
    hp -= damage;
}

qint8 Unit::getRace()
{
    return race;
}

QString Unit::getRaceString()
{
    switch(race)
    {
    case Black:
        return "Black";
    case Green:
        return "Green";
    case Red:
        return "Red";
    case White:
        return "White";
    default:
        return "Undefined";
    }
}

QString Unit::getName()
{
    return name;
}

int Unit::getPrice()
{
    return price;
}

QString Unit::getDescription()
{
    return description;
}

int Unit::getMovesLeft()
{
    return movesLeft;
}

int Unit::getMaxMoves()
{
    return maxMoves;
}

int Unit::getAttack()
{
    return attack;
}

int Unit::getHp()
{
    return hp;
}

int Unit::getArmor()
{
    return armor;
}

int Unit::getRange()
{
    return range;
}

int Unit::getType()
{
    return type;
}

QPixmap Unit::getImage()
{
    return image;
}

QPoint Unit::getLocation()
{
    return location;
}
