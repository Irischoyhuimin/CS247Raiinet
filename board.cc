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
    if(x < 0 || x >= size) {
        if(&player == link.getOwner()){
            if(player.getName() == "Player1" && x == size) return false;
            if(player.getName() == "Player2" && x == -1) return false;
        }
        return true;
    }
    if(y < 0 || y >= size) return true;

    if (grid[x][y].isServerPort()) {
        return true;
    } 
    if (grid[x][y].getLink() && grid[x][y].getLink()->getOwner() == &player) {
        return true;
    } 
    return false;
}

void Board::move(Player* activePlayer, Player* inactivePlayer, Link& link, int x, int y) {
    // Find the current cell of `link`
    Cell* oldCell = nullptr;
    for (auto& row : grid) {
        for (auto& cell : row) {
            if (cell.getLink() == &link) {
                oldCell = &cell;
                break;
            }
        }
        if (oldCell) break;
    }
    if (!oldCell) return;

    // --- OFF-BOARD downloads (handle before touching grid[x][y]) ---
    // Player1 moves OFF the bottom edge -> Player1 downloads own link
    // Player2 moves OFF the top edge    -> Player2 downloads own link
    if (x < 0 || x >= size) {
        if (activePlayer->getName() == "Player1" && x == size) {
            activePlayer->downloadLink(&link);
            oldCell->removeLink();
            return;
        }
        if (activePlayer->getName() == "Player2" && x == -1) {
            activePlayer->downloadLink(&link);
            oldCell->removeLink();
            return;
        }
        // any other off-board attempt is ignored
        return;
    }

    // Horizontal off-board is never allowed (guard; isInvalidMove should block it)
    if (y < 0 || y >= size) return;

    // --- Moving INTO a server port ---
    if (grid[x][y].isServerPort()) {
        // Opponent downloads your link if you step on their port
        // (own ports should have been disallowed in isInvalidMove)
        inactivePlayer->downloadLink(&link);
        oldCell->removeLink();
        return;
    }

    // --- Normal move / battle ---
    Link* target = grid[x][y].getLink();

    if (target && target->getOwner() != activePlayer) {
        // Battle: decide winner; download loser; place winner correctly
        Link* winner = battle(*activePlayer, *inactivePlayer, link, *target);

        // The attacker has left old cell in any case
        oldCell->removeLink();

        if (winner == &link) {
            // Attacker won: occupies the target cell
            grid[x][y].setLink(&link);
        } else {
            // Defender won: defender already sits in grid[x][y], do nothing
            // (If you mark loser downloaded inside battle, ensure the cell holding the loser is cleared there or here)
        }
    } else {
        // Empty destination (or own piece—should be blocked by isInvalidMove)
        grid[x][y].setLink(&link);
        oldCell->removeLink();
    }
}

Link* Board::battle(Player& p1, Player& p2, Link& l1, Link& l2) {
    p1.revealLink(l2.getId());
    p2.revealLink(l1.getId());
    if (l1.getStrength() >= l2.getStrength()) {
        p2.downloadLink(&l2);
        return &l1;  // l1 wins
    } else {
        p1.downloadLink(&l1);
        return &l2;  // l2 wins
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

    char id = (player.getName() == "Player1") ? 'a' : 'A';

    vector<pair<int, int>> positions;
    if (player.getName() == "Player1") {
        positions = {{0, 0}, {0, 1}, {0, 2}, {1, 3}, {1, 4}, {0, 5}, {0, 6}, {0, 7}};
    } else {
        positions = {{7, 0}, {7, 1}, {7, 2}, {6, 3}, {6, 4}, {7, 5}, {7, 6}, {7, 7}};
    }

    for (int i = 0; i < 8; ++i) {
        string t = tokens[i];
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

        auto [row, col] = positions[i];
        grid[row][col].setLink(link);

        ++id;
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

int Board::getSize() const {
    return size;
}