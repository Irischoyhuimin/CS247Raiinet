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
    for(int i = 0; i < size-1; i++) {
        cout << "==";
    }
    cout << "=";
    std::cout << "\n";
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            Link* link = (*grid)[i][j].getLink();
            char toPrint = '.';
            if (link) {
                toPrint = link->getId();
            } else if ((*grid)[i][j].isServerPort()) {
                toPrint = 'S';
            }
            std::cout << toPrint << " ";
        }
        std::cout << "\n";
    }
    for(int i = 0; i < size-1; i++) {
        cout << "==";
    }
    cout << "=";
    std::cout << "\n\n";
}
