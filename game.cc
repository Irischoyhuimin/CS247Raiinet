#include "game.h"
#include <iostream>

Game::Game(bool textOnly)
    : board{}
    , player1("Player1")
    , player2("Player2")
    , activePlayer(&player1)
    , inactivePlayer(&player2)
    , enhancementsOn(false)
    , textOnly(textOnly)
    , xw(nullptr)
{}

Game::Game(Xwindow* xw)
    : board(*xw)
    , player1("Player1")
    , player2("Player2")
    , activePlayer(&player1)
    , inactivePlayer(&player2)
    , enhancementsOn(false)
    , textOnly(false)
    , xw(xw)
{}

void Game::start() {
    board.setup();
    display();
    std::cout << "Game started.";
}

// void Game::init() {
//     // Initialization logic if needed (stub)
// }

// void Game::toggleEnhancements() {
//     enhancementsOn = !enhancementsOn;
//     board.toggleEnhancementsOn();
// }

void Game::move(const std::string& linkId, const std::string& dir) {
    if (linkId.empty()) return;
    Link* link = activePlayer->getLinkById(linkId[0]);
    if (!link || link->downloaded()) {
        std::cout << "Invalid or downloaded link.";
        return;
    }

    int currX = -1, currY = -1;
    auto grid = board.getGrid();
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if ((*grid)[i][j].getLink() == link) {
                currX = i; currY = j;
                break;
            }
        }
        if (currX != -1) break;
    }
    if (currX == -1) {
        std::cout << "Link not found on board.";
        return;
    }

    int dx = 0, dy = 0;
    if (dir == "up") dx = -1;
    else if (dir == "down") dx = 1;
    else if (dir == "left") dy = -1;
    else if (dir == "right") dy = 1;
    else {
        std::cout << "Invalid direction.";
        return;
    }

    int newX = currX + dx;
    int newY = currY + dy;
    if (board.isInvalidMove(*link, newX, newY, *activePlayer)) {
        std::cout << "Illegal move.";
        return;
    }

    board.move(activePlayer, inactivePlayer, *link, newX, newY);

    if (activePlayer->getDataDownloaded() >= 4) {
        std::cout << activePlayer->getName() << " wins by downloading 4 data!";
        exit(0);
    }
    if (activePlayer->getVirusDownloaded() >= 4) {
        std::cout << activePlayer->getName() << " loses by downloading 4 viruses.";
        exit(0);
    }

    switchPlayers();
    display();
}

void Game::useAbility(const std::string& ability, const std::vector<std::string>& args) {
    int id = std::stoi(ability);
    activePlayer->useAbility(id - 1, args);
}

void Game::display() const {
    std::cout << "Current Board State:" << std::endl;
    board.printTextDisplay();
    activePlayer->printStatus();
    inactivePlayer->printStatus();
}

bool Game::isGameOver() const {
    return activePlayer->getDataDownloaded() >= 4 ||
           activePlayer->getVirusDownloaded() >= 4;
}

void Game::switchPlayers() {
    std::swap(activePlayer, inactivePlayer);
}
