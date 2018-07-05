#include "game.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Game *widget = new Game;
    widget->show();
    return a.exec();
}
