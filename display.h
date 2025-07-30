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
    void update() override;
    void print();
};

class GraphicsDisplay : public Observer {
    Board* board;
    Xwindow* window;
    int cellSize;  // pixels per cell
    int backgroundColor;
    int firewallColorP1;
    int firewallColorP2;
    int serverPortColor;
    int linkColorP1;
    int linkColorP2;
    int boardOffsetY;
public:
    GraphicsDisplay(Board* b, Xwindow* xw);
    void update() override;
    void update(const Player* viewer, const Player* opponent);
    void redrawCell(int row, int col, const Player* viewer);
    void redrawBoard(const Player* viewer, const Player* opponent) ;
    void drawLink(int row, int col, Link* link, bool revealed);
    void drawPlayerStatus(const Player& player, const Player* viewer, int x, int y);
    void drawGrid();
};
#endif
