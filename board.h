#ifndef BOARD_H
#define BOARD_H

#include "field.h"
#include "unit.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtCore/qmath.h>

class Board : public QGraphicsView //class for managing display on board
{
    Q_OBJECT

private:

    //QVector<QPolygonF> fields;
    static const int w = 9; //board width (row)     width of a hex = a * 2
    static const int h = 8; //board height (column) height of a hex = a * sqrt(3)
    static const int a = 35; //field border length
    static const float x = a * 1.5; //vertical distance between two elements:       vertical distance is 1.5 border distance
    static const float y = a * 0.86603; //horizontal distance between two elements  shortened from the Law of cosines: y = sqrt(a^2+a^2-2*a*a*cos120)
    static const float board_width = a * (w * 1.5 + 0.5) + 1; //width = number of elements in row * x 0.5 * a + 1 ==> includes fraction part
    static const float board_height = a * (0.86603 * (2 * h + 1)) + 1; //height = number of elements in column * y * 2 - y + 1 => includes fraction part
    //Unit selected_field;
    int selected;
    quint8 currentPlayer;
    QList<QPolygonF> *validHexes; //moves or attack
    QList<QPoint> *coloredHexes;
    //int unitID;

    void mousePressEvent(QMouseEvent* event);
    //void mouseReleaseEvent(QMouseEvent *event);
    QPolygonF addHexagon(QPointF center);
    QPointF indexToBoard(int i, int j); //transforms indexed location ([0][0]) to location on scene
    int clickedHexContainsUnit(Unit &unit);
    void selectHex(QPen pen, int x, int y);
    void moveUnit(Unit &unit, QPoint target);
    int attackUnit(Unit &attacker, Unit &target); //add type of attack move (charge, skirmish)
    //void unselectHex(int x, int y);
    QList<QPolygonF> hexesInRange(int range, QPoint loc);
    QPolygonF moveUp(QPoint loc);
    QPolygonF moveDown(QPoint loc);
    QPolygonF moveLeftUp(QPoint loc);
    QPolygonF moveLeftDown(QPoint loc);
    QPolygonF moveRightUp(QPoint loc);
    QPolygonF moveRightDown(QPoint loc);

public:
    Board();
    QGraphicsScene* scene;
    QPolygonF fields[w][h];
    //Unit units[w][h]; //an array of units on the board. could be more efficient? -> problem with stacking units. no stacking? -> perhaps add units inside a unit? and then units inside that unit
    QList<Unit> activeUnits; //units on board

    void addUnit(Unit &unit, QPoint loc, bool is_move=false);
    void removeObject(Unit &unit, bool is_unit=false, bool is_move=false);
    QSize getSize();
    void setCurrentPlayer(qint8 player);

signals:
    //void sendUpdateBoard();
    void appendOutputText(QString text);
    void showUnitDescription(Unit unit);
};

#endif // BOARD_H
