#include "game.h"
#include "ui_game.h"
#include <QDebug>
#include <QFile>
#include <QtXml>
#include <QMessageBox>
#include <QSettings>

Game::Game(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);

    settings = new Settings(parent);
    QCoreApplication::setOrganizationName("Martin");
    QCoreApplication::setApplicationName("The Game");

    // forwarding the check status
    connect(ui->actionFullscreen,SIGNAL(triggered(bool)),this,SLOT(slotForwardCheckSignal()));
    connect(this,SIGNAL(fullscreenChecked()),this,SLOT(showFullScreen()));
    connect(this,SIGNAL(fullscreenUnchecked()),this,SLOT(showNormal()));
    connect(ui->actionSettings,SIGNAL(triggered(bool)),this,SLOT(showSettings()));
    connect(ui->actionNew_game,SIGNAL(triggered(bool)),this,SLOT(newGame()));
    connect(ui->pushButtonEndTurn,SIGNAL(clicked()),this,SLOT(startTurn()));
    connect(ui->comboBoxUnit,SIGNAL(currentTextChanged(QString)),this,SLOT(showUnitData()));
    connect(ui->pushButtonSummon,SIGNAL(clicked()),this,SLOT(summonUnit()));

    view = new Board();
    //connect(view,SIGNAL(sendUpdateBoard()),this,SLOT(receiveUpdateBoard()));
    connect(view,SIGNAL(appendOutputText(QString)),ui->plainTextEditOutput,SLOT(appendPlainText(QString)));
    connect(view,SIGNAL(showUnitDescription(Unit)),this,SLOT(showUnitData(Unit)));

    view->setParent(parent);
    view->setFixedHeight(view->scene->height());
    view->setFixedWidth(view->scene->width());
    receiveUpdateBoard();

    ui->layoutBoard->addWidget(view);
    init();
}

void Game::receiveUpdateBoard()
{
    view->setScene(view->scene);
    //view->update();
}

Game::~Game()
{
    delete ui;
}

void Game::showSettings()
{
    settings->show();
}

void Game::slotForwardCheckSignal()
{
    if( ui->actionFullscreen->isChecked() )
        // the action is checked
        emit fullscreenChecked();
    else
        // the action is unchecked
        emit fullscreenUnchecked();
}

void Game::loadUnits()
{   
    if(playerList[Black])
        readXmlFile("data/units/black.xml",Black);
    if(playerList[Green])
        readXmlFile("data/units/green.xml",Green);
    if(playerList[Red])
        readXmlFile("data/units/red.xml",Red);
    if(playerList[White])
        readXmlFile("data/units/white.xml",White);
}

void Game::newGame()
{
    foreach(Unit unit,view->activeUnits)
        view->removeObject(unit,true);

    view->activeUnits.clear();

    clearContainers();
    init();
}

void Game::init()
{
    QSettings settings;

    playerList[Black] = playerList[White] = true;
    //playerList[Black] = settings.value("player/black").toBool();
    playerList[Green] = settings.value("player/green").toBool();
    playerList[Red] = settings.value("player/red").toBool();
    //playerList[White] = settings.value("player/white").toBool();
    int count=0;
    for(int i=0;i<num_of_players;i++)
    {
        if(playerList[i])
            count++;
    }
    if(count<2)
    {
        playerList[Black] = playerList[White] = true;
        playerList[Green] = playerList[Red] = false;
    }

    loadUnits();

    QSize tmp = view->getSize();
    QPoint loc;

    if(playerList[Black])
    {
        loc = QPoint(0,0);
        addUnit(unitListBlack[0],loc);
        unitListBlack[0].setLocation(loc);
        funds[Black] = startingFunds;
        //view->addUnit(unitListBlack[0],view->fields[0][0]);
        //activeUnits.append(Unit(unitListBlack[0],QPoint(0,0)));
    }

    if(playerList[Green])
    {
        loc = QPoint(tmp.width()-1,0);
        addUnit(unitListGreen[0],loc);
        unitListGreen[0].setLocation(loc);
        funds[Green] = startingFunds;
        //view->addUnit(unitListGreen[0],view->fields[tmp.width()-1][0]);
        //activeUnits.append(Unit(unitListGreen[0],QPoint(tmp.width()-1,0)));
    }

    if(playerList[Red])
    {
        loc = QPoint(0,tmp.height()-1);
        addUnit(unitListRed[0],loc);
        unitListRed[0].setLocation(loc);
        funds[Red] = startingFunds;
        //view->addUnit(unitListRed[0],view->fields[0][tmp.height()-1]);
        //activeUnits.append(Unit(unitListRed[0],QPoint(0,tmp.height()-1)));
    }

    if(playerList[White])
    {
        loc = QPoint(tmp.width()-1,tmp.height()-1);
        addUnit(unitListWhite[0],loc);
        unitListWhite[0].setLocation(loc);
        funds[White] = startingFunds;
        //view->addUnit(unitListWhite[0],view->fields[tmp.width()-1][tmp.height()-1]);
        //activeUnits.append(Unit(unitListWhite[0],QPoint(tmp.width()-1,tmp.height()-1)));
    }

    //set first player
    if(settings.value("player/black").toBool())
        player = Black;
    else if(settings.value("player/green").toBool())
        player = Green;
    else if(settings.value("player/red").toBool())
        player = Red;
    else
        qDebug() << "Not enough players. Check settings";

    //currentPlayer();
    startTurn();

    //view->removeUnit(unitListRed[0]);
    //view->scene->addItem(item);
}

void Game::addUnit(Unit &unit, QPoint loc)
{
    view->addUnit(unit,loc);
    //view->activeUnits.append(Unit(unit,loc));
}

void Game::summonUnit()
{
    //qDebug() << current[0].getLocation();
    Unit unit = current[ui->comboBoxUnit->currentIndex()+1];
    
    if(unit.getPrice() > funds[unit.getRace()])
    {
        ui->plainTextEditOutput->appendPlainText(tr("You can't afford that unit!"));
        return;
    }
    
    QPoint loc;
    
    updateFunds(unit.getPrice(),unit.getRace());
    
    if(Black == unit.getRace())
        loc = unitListBlack[0].getLocation();
    else if(Green == unit.getRace())
        loc = unitListGreen[0].getLocation();
    else if(Red == unit.getRace())
        loc = unitListRed[0].getLocation();
    else if(White == unit.getRace())
        loc = unitListWhite[0].getLocation();
    else
        qDebug() << "Location not found. Valid values 0-3. Value: " << loc;
    qDebug() << loc;
    addUnit(current[ui->comboBoxUnit->currentIndex()+1],loc);
}

void Game::currentPlayer()
{
    if(player < 0 || player > 3 || !playerList[player]) //if selected player isn't active choose next player
    {
        //qDebug() << "noplayer";
        if(player < 3)
            player++;
        else
            player=-1;
        currentPlayer(); //increments player before calling function again
        return;
    }

    view->setCurrentPlayer(player);

    switch(player)
    {
    case Black:
        current = unitListBlack;
        ui->labelPlayer->setText("Black");
        break;
    case Green:
        current = unitListGreen;
        ui->labelPlayer->setText("Green");
        break;
    case Red:
        current = unitListRed;
        ui->labelPlayer->setText("Red");
        break;
    case White:
        current = unitListWhite;
        ui->labelPlayer->setText("White");
        break;
    default:
        qDebug() << "Error choosing player. Valid 0-3, current: " << player;
        break;
    }
    player++;
}

void Game::clearContainers()
{
    ui->comboBoxUnit->clear();
    ui->plainTextEditOutput->clear();
}

void Game::startTurn()
{
    //clearContainers();
    ui->comboBoxUnit->clear();

    int countPlayers=0;
    for(int i=0; i<4; i++)
    {
        if(playerList[i])
            countPlayers++;
    }

    if(1==countPlayers)
    {
        QMessageBox Victory;
        Victory.setText(QT_TR_NOOP("Game over! Player number " + QString::number(countPlayers) + " wins!"));
        return;
    }

    //qDebug() << "before" << player;
    currentPlayer(); //gets current player

    for(int i=0;i<view->activeUnits.count();i++)
    {
        if(view->activeUnits[i].getRace() == current[0].getRace())
        {
            qDebug() << "Resetting moves left";
            view->activeUnits[i].setMovesLeft(view->activeUnits[i].getMaxMoves());
        }
    }

    QStringList list;
    foreach(Unit unit,current)
    {
        list.append(unit.getName());
    }
    list.removeAt(0); //remove fortress

    updateFunds(funds[current[0].getRace()]);

    ui->comboBoxUnit->addItems(list);
}

void Game::updateFunds(int change, qint8 race)
{
    if(-1 != race)
    {
        funds[race]-=change; //update funds
        ui->lineEditFunds->setText(QString::number(funds[race]));
    }
    else
    {
        ui->lineEditFunds->setText(QString::number(change));
    }
}

void Game::showUnitData(Unit unit)
{
    if(unit.getRace() == -1)
        unit = current[ui->comboBoxUnit->currentIndex()+1];
    ui->plainTextEditInfo->setPlainText(unit.getDescription());
    ui->plainTextEditInfo->appendPlainText("------------------------------");
    ui->plainTextEditInfo->appendPlainText(tr("Price:\t") + QString::number(unit.getPrice()));
    ui->plainTextEditInfo->appendPlainText("------------------------------");
    ui->plainTextEditInfo->appendPlainText(tr("Moves:\t") + QString::number(unit.getMovesLeft()));
    ui->plainTextEditInfo->appendPlainText("------------------------------");
    ui->plainTextEditInfo->appendPlainText(tr("AP:\t") + QString::number(unit.getAttack()));
    ui->plainTextEditInfo->appendPlainText("------------------------------");
    ui->plainTextEditInfo->appendPlainText(tr("HP:\t") + QString::number(unit.getHp()));
    ui->plainTextEditInfo->appendPlainText("------------------------------");
    ui->plainTextEditInfo->appendPlainText(tr("Armor:\t") + QString::number(unit.getArmor()));
    ui->plainTextEditInfo->appendPlainText("------------------------------");
    ui->plainTextEditInfo->appendPlainText(tr("Range:\t") + QString::number(unit.getRange()));
}

void Game::readXmlFile(QString path, qint8 iteration)
{
    QDomDocument xml;
    QString unitName;
    //QList<QString> names; //not necessary, would be enough to read once
    QList<QString> atts; //attributes
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Error reading file " + path;
        return;
    }
    xml.setContent(&file);

    QDomElement root = xml.documentElement();
    if("black" == root.tagName() || "green" == root.tagName() || "red" == root.tagName() || "white" == root.tagName())
    {
        QDomElement unit = root.firstChild().toElement();

        int unitCount = 0; //for images
        while (!unit.isNull())
        {
            unitName = unit.tagName(); //unit name

            QDomElement attribute = unit.firstChild().toElement(); //first stat

            //qDebug() << unitName;
            while(!attribute.isNull())
            {
                //names.append(attribute.tagName());
                atts.append(attribute.firstChild().toText().data()); //attribute data
                attribute = attribute.nextSibling().toElement(); //next
            }
            unit = unit.nextSibling().toElement();
            //qDebug() << attributes;

            QString imgPath = "data/images/" + root.tagName() + QString::number(unitCount) + ".png";

            if(0==iteration)
                unitListBlack.append(Unit(iteration, unitName, atts[0].toInt(), atts[1], atts[2].toInt(), atts[3].toInt(), atts[4].toInt(), atts[5].toInt(), atts[6].toInt(), atts[7].toInt(), QPixmap(imgPath)));
            else if(1==iteration)
                unitListGreen.append(Unit(iteration, unitName, atts[0].toInt(), atts[1], atts[2].toInt(), atts[3].toInt(), atts[4].toInt(), atts[5].toInt(), atts[6].toInt(), atts[7].toInt(), QPixmap(imgPath)));
            else if(2==iteration)
                unitListRed.append(Unit(iteration, unitName, atts[0].toInt(), atts[1], atts[2].toInt(), atts[3].toInt(), atts[4].toInt(), atts[5].toInt(), atts[6].toInt(), atts[7].toInt(), QPixmap(imgPath)));
            else if(3==iteration)
                unitListWhite.append(Unit(iteration, unitName, atts[0].toInt(), atts[1], atts[2].toInt(), atts[3].toInt(), atts[4].toInt(), atts[5].toInt(), atts[6].toInt(), atts[7].toInt(), QPixmap(imgPath)));
            else
            {
                QMessageBox Error;
                Error.setText(QT_TR_NOOP("Faction not found! Iteration number " + QString::number(iteration) + ". Expected 0-3"));
                Error.exec();
            }
            atts.clear(); //clear attributes for next unit
            unitCount++;
        }
    }
    else
    {
        QMessageBox Error;
        Error.setText(QT_TR_NOOP("File '" + path + "' not recognised!"));
        Error.exec();
    }
    file.close();
}
