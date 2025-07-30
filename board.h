#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
#include "cell.h"
#include "link.h"
#include "player.h"

class TextDisplay;      
class GraphicsDisplay;  
class Xwindow;  
class Board {
    static const int size = 8;
    vector<vector<Cell>> grid;
    TextDisplay* td;
    GraphicsDisplay* gd;
    set<pair<int,int>> player1Firewalls;
    set<pair<int,int>> player2Firewalls;
public:
    Board();
    Board(Xwindow& xw);
    bool isInvalidMove(const Link& link, int x, int y, const Player& player) const;
    void move(Player* activePlayer, Player* inactivePlayer, Link& link, int x, int y);
    Link* battle(Player& p1, Player& p2, Link& l1, Link& l2);
    void setup();
    void setupLinks(Player& player, const string& config);
    void printTextDisplay() const;
    vector<vector<Cell>>* getGrid();
    GraphicsDisplay* getGD() const;
    bool canPlaceFirewallAt(int x, int y) const;
    void placeFirewall(int x, int y, Player* owner);
    bool opponentHasFireWallAt(int x, int y, Player* opponent) const;
    bool isOccupiedByOpponent(Player* opponent, int x, int y) const;
    bool isServerPort(int x, int y) const;
    int getSize() const;
    ~Board();
    bool swapLinks(Link* l1, Link* l2);
    void replaceLink(Link* oldL, Link* newL);
    Cell* findCellOf(Link* l);
    bool findRespawnSquare(Player* owner, int& outRow, int& outCol, Player* opponent = nullptr);
    void placeLink(Link* l, int row, int col);
};
#endif
