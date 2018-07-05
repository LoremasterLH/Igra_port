#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "settings.h"
#include "colors.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVector>

namespace Ui {
class Game;
}

class Game : public QMainWindow
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = 0);
    ~Game();

private:
    Ui::Game *ui;
    Board* view;
    Settings* settings;
    QList<Unit> unitListBlack;
    QList<Unit> unitListGreen;
    QList<Unit> unitListRed;
    QList<Unit> unitListWhite;

    //QList<Unit> fieldUnitListBlack;
    //QList<Unit> fieldUnitListGreen;
    //QList<Unit> fieldUnitListRed;
    //QList<Unit> fieldUnitListWhite;

    
    //QList<Unit> activeUnits; //units on board
    QList<Unit> current; //current player
    qint8 player;
    static const int startingFunds = 10;

    static const int num_of_players = 4;
    bool playerList[num_of_players]; //0 = black, 1 = green, 2 = red, 3 = white; true = player active
    int funds[num_of_players];

    void clearContainers();
    void loadUnits();
    void init(); //initialise starting board position
    void readXmlFile(QString path, qint8 iteration); //to determine which faction's turn it is
    void currentPlayer(); //0 = black, 1 = green, 2 = red, 3 = white
    void addUnit(Unit &unit, QPoint loc);
    void updateFunds(int change, qint8 race=-1); //to update actual funds and displayed funds at the same time

signals:
    void fullscreenChecked(); //emitted when action is checked
    void fullscreenUnchecked(); //emitted when action is NOT checked

private slots:
    void slotForwardCheckSignal(); //forwaerd triggered event depending on status (checked, unchecked)
    void receiveUpdateBoard();
    void showSettings();
    void newGame();
    void startTurn();
    void showUnitData(Unit unit=Unit());
    void summonUnit();
};

#endif // GAME_H
