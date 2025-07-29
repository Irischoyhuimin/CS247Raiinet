#ifndef DISPLAY_H
#define DISPLAY_H
#include "observer.h"
#include "player.h"
class Board;
class Xwindow;

class TextDisplay : public Observer {
    Board* board;
    const Player* activePlayer = nullptr;
    const Player* opponentPlayer = nullptr;

public:
    TextDisplay(Board* b);
    void setPlayers(const Player* active, const Player* opponent);
    void update() override;
    void print();
};

class GraphicsDisplay : public Observer {
    Board* board;
    Xwindow* window;

public:
    GraphicsDisplay(Board* b, Xwindow* xw);
    void update() override;
    void redrawCell(int x, int y);
    void redrawBoard();
};
#endif
