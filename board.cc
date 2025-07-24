#include "board.h"
#include "display.h"    
#include "xwindow.h"
#include <sstream>
#include <iostream>
using namespace std; 

Board::Board() : td{new TextDisplay(this)}, gd{nullptr} {
    for (int i = 0; i < size; ++i) {
        vector<Cell> row;
        for (int j = 0; j < size; ++j) {
            row.emplace_back(i, j);
        }
        grid.push_back(row);
    }
}

Board::Board(Xwindow& xw) : td{new TextDisplay(this)}, gd{new GraphicsDisplay(this, &xw)} {
    for (int i = 0; i < size; ++i) {
        vector<Cell> row;
        for (int j = 0; j < size; ++j) {
            row.emplace_back(i, j);
        }
        grid.push_back(row);
    }
}

bool Board::isInvalidMove(const Link& link, int x, int y, const Player& player) const {
    if (x < 0 || y < 0 || x >= size || y >= size){
        return true;
    } 
    if (grid[x][y].isServerPort()) {
        return true;
    } 
    if (grid[x][y].getLink() && grid[x][y].getLink()->getOwner() == &player) {
        return true;
    } 
    return false;
}

void Board::move(Player* activePlayer, Player* inactivePlayer, Link& link, int x, int y) {
    Cell* oldCell = nullptr;
    for (auto& row : grid) {
        for (auto& cell : row) {
            if (cell.getLink() == &link) {
                oldCell = &cell;
                break;
            }
        }
    }
    if (!oldCell) return;

    Link* target = grid[x][y].getLink();
    if (target && target->getOwner() != activePlayer) {
        battle(*activePlayer, *inactivePlayer, link, *target);
        oldCell->removeLink();
    } else {
        grid[x][y].setLink(&link);
        oldCell->removeLink();
    }
    td->update();
    if (gd) gd->update();
}

void Board::battle(Player& p1, Player& p2, Link& l1, Link& l2) {
    if (l1.getStrength() >= l2.getStrength()) {
        l2.setDownloaded(true);
    } else {
        l1.setDownloaded(true);
    }
}

void Board::setup() {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if ((i == 0 || i == size - 1) && (j == 3 || j == 4)) {
                grid[i][j].setServerPort(true);
            }
        }
    }
}

void Board::setupLinks(Player& player, const string& config) {
    istringstream iss(config);
    vector<string> tokens;
    string token;

    while (iss >> token) {
        tokens.push_back(token);
    }

    if (tokens.size() != 8) {
        cerr << "Invalid link config: must have 8 tokens\n";
        return;
    }

    int row = (player.getName() == "Player1") ? 1 : 6;
    char id = (player.getName() == "Player1") ? 'a' : 'A';

    int col = 0;
    for (int i = 0; i < 8; ++i) {
        string t = tokens[i];
        if (col == 3 || col == 4) ++col; 

        char type = t[0];      
        int strength = t[1] - '0';

        Link* link = nullptr;
        if (type == 'V') {
            link = new Virus(id, &player, strength);
        } else if (type == 'D') {
            link = new Data(id, &player, strength);
        } else {
            cerr << "Invalid link type: " << type << "\n";
            return;
        }

        player.addLink(link);                 
        grid[row][col].setLink(link);         
        ++id;
        ++col;
    }
}

void Board::printTextDisplay() const {
    td->print();
}

vector<vector<Cell>>* Board::getGrid() {
    return &grid;
}

GraphicsDisplay* Board::getGD() {
    return gd;
}

bool Board::opponentHasFireWallAt(int x, int y, Player* opponent) const {
    if (x < 0 || y < 0 || x >= size || y >= size) return false;
    return grid[x][y].hasFirewall() && grid[x][y].getFirewallOwner() == opponent;
}

bool Board::isOccupiedByOpponent(Player* opponent, int x, int y) const {
    if (x < 0 || y < 0 || x >= size || y >= size) return false;
    Link* link = grid[x][y].getLink();
    return link && link->getOwner() == opponent;
}

bool Board::isServerPort(int x, int y) const {
    return grid[x][y].isServerPort();
}
