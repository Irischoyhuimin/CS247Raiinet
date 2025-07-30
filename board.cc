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
    // Locate current cell
    Cell* oldCell = nullptr;
    int oldX = -1, oldY = -1;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (grid[i][j].getLink() == &link) {
                oldCell = &grid[i][j];
                oldX = i; oldY = j;
                break;
            }
        }
        if (oldCell) break;
    }
    if (!oldCell) return;

    // ---------- OFF-BOARD downloads (must happen BEFORE touching grid[x][y]) ----------
    // Moving off the far edge towards opponent = download your own link
    if (x < 0 || x >= size) {
        // Player1 escapes off bottom edge (x == size)
        if (activePlayer->getName() == "Player1" && x == size) {
            activePlayer->downloadLink(&link);
            oldCell->removeLink();
            return;
        }
        // Player2 escapes off top edge (x == -1)
        if (activePlayer->getName() == "Player2" && x == -1) {
            activePlayer->downloadLink(&link);
            oldCell->removeLink();
            return;
        }
        // Any other out-of-bounds is invalid (guard)
        return;
    }

    // Horizontal off-board guard (should be blocked by isInvalidMove anyway)
    if (y < 0 || y >= size) return;

    // ---------- Server port: stepping on opponent port -> opponent downloads you ----------
    if (grid[x][y].isServerPort()) {
        inactivePlayer->downloadLink(&link);
        oldCell->removeLink();
        return;
    }

    // ---------- Boosted mid-square firewall check (passing THROUGH) ----------
    if (link.hasBoost()) {
        int dx = (x > oldX) ? 1 : (x < oldX) ? -1 : 0;
        int dy = (y > oldY) ? 1 : (y < oldY) ? -1 : 0;
        int midX = oldX + dx;
        int midY = oldY + dy;
        if (midX >= 0 && midX < size && midY >= 0 && midY < size) {
            if (opponentHasFireWallAt(midX, midY, inactivePlayer)) {
                // Reveal attacker to both sides
                inactivePlayer->revealLink(link.getId());
                activePlayer->revealLink(link.getId());
                if (link.getType() == "V") {
                    // Virus hits firewall -> attacker is downloaded by its owner
                    activePlayer->downloadLink(&link);
                    oldCell->removeLink();
                    // Firewall persists; do NOT remove it
                    return;
                }
                // Data: continue to destination
            }
        }
    }

    // ---------- Destination firewall check (applies BEFORE battle) ----------
    // If the destination square has opponent's firewall, it triggers first.
    if (opponentHasFireWallAt(x, y, inactivePlayer)) {
        // Reveal attacker to both sides
        inactivePlayer->revealLink(link.getId());
        activePlayer->revealLink(link.getId());
        if (link.getType() == "V") {
            // Virus is immediately downloaded; no battle occurs
            activePlayer->downloadLink(&link);
            oldCell->removeLink();
            return;
        }
        // If attacker is Data, proceed. If there is a defender, we will battle next.
        // Firewall remains under the piece if it ends up standing there.
    }

    // ---------- Normal move / battle ----------
    Link* target = grid[x][y].getLink();
    if (target && target->getOwner() != activePlayer) {
        // Battle (firewall already applied if present)
        Link* winner = battle(*activePlayer, *inactivePlayer, link, *target);

        // Attacker vacated old cell
        oldCell->removeLink();

        if (winner == &link) {
            // Attacker takes the square
            grid[x][y].setLink(&link);
        }
        // Else defender remains; nothing to do
    } else {
        // Empty destination (or own piece would have been blocked earlier)
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

GraphicsDisplay* Board::getGD() const{
    return gd;
}

bool Board::canPlaceFirewallAt(int x, int y) const {
    if (x < 0 || y < 0 || x >= size || y >= size) return false;
    if (grid[x][y].isServerPort()) return false;   // spec: not on ports
    if (grid[x][y].isOccupied())   return false;   // must be empty square
    if (grid[x][y].hasFirewall())  return false;   // don’t stack
    return true;
}

void Board::placeFirewall(int x, int y, Player* owner) {
    if (!canPlaceFirewallAt(x, y)) {
        std::cout << "Cannot place firewall at (" << x << "," << y << ").\n";
        return;
    }
    grid[x][y].setFirewall(owner);
    // Text/graphics displays will react via notifyObservers() from Cell::setFirewall
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

Board::~Board() {
    delete td;  
    delete gd;  
}

bool Board::swapLinks(Link* l1, Link* l2) {
    if (!l1 || !l2) return false;

    Cell* cell1 = nullptr;
    Cell* cell2 = nullptr;

    // Find the cells holding l1 and l2
    for (auto& row : grid) {
        for (auto& cell : row) {
            if (cell.getLink() == l1) cell1 = &cell;
            else if (cell.getLink() == l2) cell2 = &cell;
            if (cell1 && cell2) break;
        }
        if (cell1 && cell2) break;
    }

    if (!cell1 || !cell2) {
        std::cout << "One or both links are not on the board.\n";
        return false;
    }

    // Swap links on the two cells
    cell1->setLink(l2);
    cell2->setLink(l1);

    return true;
}

Cell* Board::findCellOf(Link* l) {
    for (auto& row : grid) {
        for (auto& cell : row) {
            if (cell.getLink() == l) return &cell;
        }
    }
    return nullptr;
}

void Board::replaceLink(Link* oldL, Link* newL) {
    Cell* cell = findCellOf(oldL);
    if (cell) {
        cell->setLink(newL);
    }
}

bool Board::findRespawnSquare(Player* owner, int& outRow, int& outCol, Player* opponent) {
    // Define the two back rows for each player
    int r1, r2;
    if (owner->getName() == "Player1") { r1 = 1; r2 = 0; }  // prefer row 1, then row 0
    else                               { r1 = 6; r2 = 7; }  // prefer row 6, then row 7

    auto tryRow = [&](int r) -> bool {
        for (int c = 0; c < size; ++c) {
            if (grid[r][c].isServerPort()) continue;     // never place on a port
            if (grid[r][c].isOccupied())   continue;     // must be empty
            // Prefer not to respawn on opponent's firewall (safer), but allow if no choice
            if (opponent && grid[r][c].hasFirewall() && grid[r][c].getFirewallOwner() == opponent) {
                continue;
            }
            outRow = r; outCol = c;
            return true;
        }
        return false;
    };

    // First pass: avoid opponent firewall
    if (tryRow(r1) || tryRow(r2)) return true;

    // Second pass: allow firewall squares if absolutely necessary
    for (int pass = 0; pass < 2; ++pass) {
        int r = (pass == 0 ? r1 : r2);
        for (int c = 0; c < size; ++c) {
            if (grid[r][c].isServerPort()) continue;
            if (grid[r][c].isOccupied())   continue;
            outRow = r; outCol = c;
            return true;
        }
    }
    return false;  // no safe square
}

void Board::placeLink(Link* l, int row, int col) {
    grid[row][col].setLink(l);
    l->setDownloaded(false);
}
