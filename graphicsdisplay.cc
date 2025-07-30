#include "display.h"
#include "board.h"
#include "cell.h"
#include "link.h"
#include "player.h"
#include "xwindow.h"

#include <string>
#include <sstream>

GraphicsDisplay::GraphicsDisplay(Board* b, Xwindow* xw)
    : board(b), window(xw),
      backgroundColor(Xwindow::White),
      firewallColorP1(Xwindow::Blue),
      firewallColorP2(Xwindow::Cyan),
      serverPortColor(Xwindow::Yellow),
      linkColorP1(Xwindow::Blue),
      linkColorP2(Xwindow::Red),
      boardOffsetY(100),
      boardOffsetX(40),
      cellSize(50) // pixels per cell
{}

// Redraw with an explicit viewer (current player)
void GraphicsDisplay::update() {
    redrawBoard(nullptr, nullptr);
}

void GraphicsDisplay::update(const Player* viewer, const Player* opponent) {
    redrawBoard(viewer, opponent);
}

void GraphicsDisplay::redrawBoard(const Player* viewer, const Player* opponent) {
    const int size = board->getSize();
    const int boardW = size * cellSize;
    const int boardH = size * cellSize;

    // Clear entire area (top status + board + bottom status)
    window->fillRectangle(0, 0, boardW, boardH + 180, backgroundColor);

    // Top: current player's full status (viewer)
    if (viewer) {
        drawPlayerStatus(*viewer, viewer, 10, 20);
    }
    
    for (int c = 0; c < size; ++c) {
        const int x = boardOffsetX + c * cellSize + cellSize / 2 - 4;
        const int y = boardOffsetY - 12;                // just above the grid
        if (y >= 0) window->drawString(x, y, std::to_string(c), Xwindow::Black);
    }

    // Row numbers (left) + cells
    for (int r = 0; r < size; ++r) {
        const int rx = boardOffsetX - 20;                                          // left of grid
        const int ry = boardOffsetY + r * cellSize + cellSize / 2 + 5;              // vertically centered
        if (rx >= 0) window->drawString(rx, ry, std::to_string(r), Xwindow::Black);

        for (int c = 0; c < size; ++c) {
            redrawCell(r, c, viewer);
        }
    }


    // Grid lines
    drawGrid();

    // Bottom: opponent’s status
    if (opponent) {
        drawPlayerStatus(*opponent, viewer, 10, boardH + 125);
    }
}


void GraphicsDisplay::redrawCell(int row, int col, const Player* viewer) {
    auto& grid = *(board->getGrid());
    Cell& cell = grid[row][col];

    const int px = boardOffsetX + col * cellSize;
    const int py = boardOffsetY + row * cellSize;

    // Background
    window->fillRectangle(px, py, cellSize, cellSize, backgroundColor);

    // Server port
    if (cell.isServerPort()) {
        window->fillRectangle(px, py, cellSize, cellSize, serverPortColor);
        window->drawString(px + cellSize / 2 - 2, py + cellSize / 2 + 5, "S", Xwindow::Black);
    }

    // Firewall (drawn under link if any)
    if (cell.hasFirewall()) {
        Player* owner = cell.getFirewallOwner();
        const bool p1 = owner && owner->getName() == "Player1";
        const int color = p1 ? firewallColorP1 : firewallColorP2;
        window->fillRectangle(px, py, cellSize, cellSize, color);
        window->drawString(px + cellSize / 2 - 2, py + cellSize / 2 + 5, p1 ? "m" : "w", Xwindow::White);
    }

    // Link
    if (Link* link = cell.getLink()) {
        // revealed if: no viewer provided OR viewer owns the link OR viewer has it revealed
        const bool revealed =
            !viewer ||
            (link->getOwner() == viewer) ||
            viewer->isLinkRevealed(link->getId());

        drawLink(row, col, link, revealed);
    }
}

void GraphicsDisplay::drawLink(int row, int col, Link* link, bool revealed) {
    const int px = boardOffsetX + col * cellSize;
    const int py = boardOffsetY + row * cellSize;

    // Colour by reveal+type: unknown=black, Data=green, Virus=red
    int color = Xwindow::Black;
    if (revealed) {
        if (link->getType() == "D")      color = Xwindow::Green;
        else if (link->getType() == "V") color = Xwindow::Red;
    }

    // Fill and label
    window->fillRectangle(px, py, cellSize, cellSize, color);
    string label(1,link->getId());
    window->drawString(px + cellSize / 2 -2, py + cellSize / 2 + 5, label, Xwindow::White);
}

void GraphicsDisplay::drawPlayerStatus(const Player& player, const Player* viewer, int x, int y) {
    // If no viewer provided, default to showing as if the player is the viewer
    const Player& v = viewer ? *viewer : player;

    string status = player.getStatusString(v);
    istringstream iss(status);
    string line;
    int lineHeight = 15;
    int i = 0;
    while (getline(iss, line)) {
        window->drawString(x, y + i * lineHeight, line, Xwindow::Black);
        ++i;
    }
}

void GraphicsDisplay::drawGrid() {
    const int size = board->getSize();
    const int w = size * cellSize;
    const int h = size * cellSize;

    // Vertical lines
    for (int c = 0; c <= size; ++c) {
        int x = c * cellSize;
        window->drawLine(boardOffsetX + x, boardOffsetY, boardOffsetX + x, h+boardOffsetY, Xwindow::Black);
    }
    // Horizontal lines
    for (int r = 0; r <= size; ++r) {
        int y = r * cellSize;
        window->drawLine(boardOffsetX, y+boardOffsetY, boardOffsetX + w, y+boardOffsetY, Xwindow::Black);
    }
}
