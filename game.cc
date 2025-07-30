#include "game.h"
#include "board.h"
#include "player.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "linkboost.h"
#include "firewall.h"
#include "download.h"
#include "scan.h"
#include "polarize.h"
#include "heal.h"
#include "mask.h"
#include "swap.h"

Game::Game(bool textOnly)
    : board{}, player1("Player1"), player2("Player2"),
      activePlayer(&player1), inactivePlayer(&player2),
      enhancementsOn(false), textOnly(textOnly), xw(nullptr) {
        vector<string> defaultOrder = {"LinkBoost", "Firewall", "Download", "Scan", "Polarize"};
        setAbilityOrder(1, defaultOrder);
        setAbilityOrder(2, defaultOrder);
      }

Game::Game(Xwindow* xw)
    : board(*xw), player1("Player1"), player2("Player2"),
      activePlayer(&player1), inactivePlayer(&player2),
      enhancementsOn(false), textOnly(false), xw(xw) {}


void Game::start() {
    board.setup();
    display();
    cout << "Game started.\n";
}

void Game::toggleEnhancements() {
    enhancementsOn = !enhancementsOn;
    cout << "[DEBUG] Enhancements turned "
              << (enhancementsOn ? "on" : "off") << ".\n";
}

void Game::move(const string& dir, const string& linkId) {
    Link* link = nullptr;
    for (const auto& lp : activePlayer->getLinks()) {
        if (lp->getId() == linkId[0]) { // assuming linkId is one char
            link = lp.get();
            break;
        }
    }
    if (!link) {
        cout << "No such link: " << linkId << "\n";
        return;
    }

    int currX = -1, currY = -1;
    auto grid = board.getGrid();
    for (int i = 0; i < (int)grid->size(); ++i) {
        for (int j = 0; j < (int)(*grid)[i].size(); ++j) {
            if ((*grid)[i][j].getLink() == link) {
                currX = i;
                currY = j;
                break;
            }
        }
        if (currX != -1) break;
    }

    if (currX == -1) {
        cout << "Link not found on board.\n";
        return;
    }

    int newX = currX, newY = currY;
    if (dir == "up") newX--;
    else if (dir == "down") newX++;
    else if (dir == "left") newY--;
    else if (dir == "right") newY++;
    else {
        cout << "Invalid direction: " << dir << "\n";
        return;
    }

    if (board.isInvalidMove(*link, newX, newY, *activePlayer)) {
        cout << "Invalid move.\n";
        return;
    }

    board.move(activePlayer, inactivePlayer, *link, newX, newY);

    switchPlayers();
}


void Game::useAbility(const string& abilityIdx, const vector<string>& args) {
    int id = stoi(abilityIdx);
    activePlayer->useAbility(id - 1, args, *inactivePlayer);
}

void Game::display() const {
    activePlayer->printStatus(*activePlayer);
    board.printTextDisplay();
    inactivePlayer->printStatus(*activePlayer);
}

bool Game::isGameOver() const {
    return activePlayer->getDataDownloaded() >= 4 ||
           activePlayer->getVirusDownloaded() >= 4 || inactivePlayer->getVirusDownloaded() >= 4 || inactivePlayer->getVirusDownloaded() >= 4;
}

void Game::switchPlayers() {
    swap(activePlayer, inactivePlayer);
}

void Game::loadLinkConfig(int playerNum, const string& filename) {
    Player* p = (playerNum == 1 ? &player1 : &player2);
    ifstream infile(filename);
    if (!infile) {
        cerr << "Failed to open link config: " << filename << "\n";
        return;
    }
    string config, line;
    while (getline(infile, line)) {
        if (!config.empty()) config += " ";
        config += line;
    }
    board.setupLinks(*p, config);
}

void Game::setAbilityOrder(int playerNum, const vector<string>& order) {
    Player* p = (playerNum == 1 ? &player1 : &player2);
    auto& abilities = p->getAbilities();
    abilities.clear();
    for (const auto& type : order) {
        if (type == "LinkBoost")      abilities.push_back(make_unique<LinkBoost>());
        else if (type == "Firewall")  abilities.push_back(make_unique<Firewall>());
        else if (type == "Download")  abilities.push_back(make_unique<Download>());
        else if (type == "Scan")      abilities.push_back(make_unique<Scan>());
        else if (type == "Polarize")  abilities.push_back(make_unique<Polarize>());
        else if (type == "Heal")      abilities.push_back(make_unique<Heal>());
        else if (type == "MaskAbility") abilities.push_back(make_unique<MaskAbility>());
        else if (type == "Swap")      abilities.push_back(make_unique<Swap>());
        else cerr << "Unknown ability: " << type << "\n";
    }
}

Player& Game::getCurrentPlayer() { return *activePlayer; }
const Player& Game::getCurrentPlayer() const { return *activePlayer; }
