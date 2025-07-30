#include "display.h"
#include "cell.h"
#include "link.h"
#include "board.h"
#include "player.h"
#include <iostream>

using namespace std;

TextDisplay::TextDisplay(Board* board) : board(board) {}

void TextDisplay::update() {
    print();
}

void TextDisplay::print() {
    int size = board->getSize();
    auto grid = board->getGrid();

    // Column header
    cout << "    ";
    for (int col = 0; col < size; ++col) {
        cout << col << " ";
    }
    cout << "\n";

    // Top border
    cout << "  ";
    for (int col = 0; col < size; ++col) {
        cout << "--";
    }
    cout << "-\n";

    for (int i = 0; i < size; ++i) {
        // Row label and left border
        cout << i << " | ";

        for (int j = 0; j < size; ++j) {
            char toPrint = '.';

            Link* link = (*grid)[i][j].getLink();
            if (link) {
                // If a link is on the square, show the link (not the firewall)
                toPrint = link->getId();
            } else if ((*grid)[i][j].isServerPort()) {
                toPrint = 'S';
            } else if ((*grid)[i][j].hasFirewall()) {
                // Empty square with a firewall: show owner-specific symbol
                Player* owner = (*grid)[i][j].getFirewallOwner();
                if (owner) {
                    const string& name = owner->getName();
                    // Spec convention: 'm' for P1 firewall, 'w' for P2 firewall
                    toPrint = (name == "Player1") ? 'm' : 'w';
                } else {
                    toPrint = 'm';
                }
            }

            cout << toPrint << ' ';
        }
        cout << '\n';
    }

    cout << "  ";
    for (int j = 0; j < size; ++j) cout << "--";
    cout << "-\n\n";
}