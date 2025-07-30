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
#include <unordered_map>
#include "display.h"
#include <cstdlib>
#include <ctime>

using namespace std;
Game::Game(bool textOnly)
    : board{}, player1("Player1"), player2("Player2"),
      activePlayer(&player1), inactivePlayer(&player2),
      enhancementsOn(false), textOnly(textOnly), xw(nullptr) {
        vector<string> defaultOrder = {"LinkBoost", "Firewall", "Download", "Scan", "Polarize"};
        setAbilityOrder(1, defaultOrder);
        setAbilityOrder(2, defaultOrder);
      }

Game::Game(Xwindow* xw) : board(*xw), player1("Player1"), player2("Player2"), activePlayer(&player1), inactivePlayer(&player2), enhancementsOn(false), textOnly(false), xw(xw) {
    vector<string> defaultOrder = {"LinkBoost", "Firewall", "Download", "Scan", "Polarize"};
    setAbilityOrder(1, defaultOrder);
    setAbilityOrder(2, defaultOrder);
}

void Game::init() {
    // No-op: initial state already configured via setup commands
}

void Game::start() {
    board.setup();
    cout << "Game started.\n";
}

void Game::toggleEnhancements() {
    enhancementsOn = !enhancementsOn;
    cout << "[DEBUG] Enhancements turned "
              << (enhancementsOn ? "on" : "off") << ".\n";
}

void Game::move(const string& dir, const string& linkId) {
    // Find the Link* owned by activePlayer with that id (case-sensitive)
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

    // Get current position of the link on the board
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

    // Calculate new coordinates based on dir
    int stepX = 0, stepY = 0;
    if (dir == "up") stepX = -1;
    else if (dir == "down") stepX = 1;
    else if (dir == "left") stepY = -1;
    else if (dir == "right") stepY = 1;
    else {
        cout << "Invalid direction: " << dir << "\n";
        return;
    }

    int newX = currX + stepX;
    int newY = currY + stepY;

    if (link->hasBoost()) {
        newX += stepX;
        newY += stepY;
    }

    // Check if move is invalid
    if (board.isInvalidMove(*link, newX, newY, *activePlayer)) {
        cout << "Invalid move.\n";
        return;
    }

    // Perform the move (handle battles inside board.move)
    board.move(activePlayer, inactivePlayer, *link, newX, newY);
    if(isGameOver()) return;
    // Optionally switch players here, or after some other phase
    switchPlayers();
}


void Game::useAbility(const string& abilityIdx, const vector<string>& args) {
    int id = stoi(abilityIdx);
    activePlayer->useAbility(id - 1, args, *inactivePlayer, board);
}

void Game::display() const {
    activePlayer->printStatus(*activePlayer);
    board.printTextDisplay();
    inactivePlayer->printStatus(*activePlayer);
    if (auto gd = board.getGD()) {
        gd->update(activePlayer, inactivePlayer);//  force GUI redraw each call to display()
     
    }
}

bool Game::isGameOver() const {
    return activePlayer->getDataDownloaded() >= 4 
        || activePlayer->getVirusDownloaded() >= 4 
        || inactivePlayer->getDataDownloaded() >= 4 
        || inactivePlayer->getVirusDownloaded() >= 4;
}

int Game::getWinner() const {
    int p1Data = player1.getDataDownloaded();
    int p1Virus = player1.getVirusDownloaded();
    int p2Data = player2.getDataDownloaded();
    int p2Virus = player2.getVirusDownloaded();

    if (p1Data >= 4) return 1;         // Player 1 wins
    if (p1Virus >= 4) return 2;        // Player 1 lost, so Player 2 wins
    if (p2Data >= 4) return 2;         // Player 2 wins
    if (p2Virus >= 4) return 1;        // Player 2 lost, so Player 1 wins

    return 0;                         // No winner yet
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
    Player& player = (playerNum == 1) ? player1 : player2;
    auto& abilities = player.getAbilities();
    abilities.clear();

    for (const string& rawType : order) {
        if (abilities.size() >= 5) break;
        
        if (rawType == "LinkBoost") {
            abilities.push_back(unique_ptr<Ability>(new LinkBoost()));
        } else if (rawType == "Firewall") {
            abilities.push_back(unique_ptr<Ability>(new Firewall()));
        } else if (rawType == "Download") {
            abilities.push_back(unique_ptr<Ability>(new Download()));
        } else if (rawType == "Scan") {
            abilities.push_back(unique_ptr<Ability>(new Scan()));
        } else if (rawType == "Polarize") {
            abilities.push_back(unique_ptr<Ability>(new Polarize()));
        } else if (rawType == "Swap") {
            abilities.push_back(unique_ptr<Ability>(new Swap()));
        } else if (rawType == "Heal") {
            abilities.push_back(unique_ptr<Ability>(new Heal()));
        } else if (rawType == "Mask") {
            abilities.push_back(unique_ptr<Ability>(new MaskAbility()));
        } else {
            cerr << "Unknown ability type: " << rawType << endl;
        }
    }
}

Player& Game::getCurrentPlayer() { return *activePlayer; }
const Player& Game::getCurrentPlayer() const { return *activePlayer; }

void Game::simpleRandomizeLinks(vector<unique_ptr<Link>>& links) {
    static bool seeded = false;
    if (!seeded) {
        srand(time(nullptr));
        seeded = true;
    }
    for (int i = links.size() - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        swap(links[i], links[j]);
    }
}

void Game::randomizeLinksForPlayer(Player& player) {
    vector<unique_ptr<Link>> links;
    links.push_back(make_unique<Data>('a', &player, 1)); // D1
    links.push_back(make_unique<Data>('b', &player, 2)); // D2
    links.push_back(make_unique<Data>('c', &player, 3)); // D3
    links.push_back(make_unique<Data>('d', &player, 4)); // D4

    links.push_back(make_unique<Virus>('e', &player, 1)); // V1
    links.push_back(make_unique<Virus>('f', &player, 2)); // V2
    links.push_back(make_unique<Virus>('g', &player, 3)); // V3
    links.push_back(make_unique<Virus>('h', &player, 4)); // V4
    simpleRandomizeLinks(links);
}

void Game::randomizeLinksForPlayer(int playerNum) {
    Player& p = (playerNum == 1) ? player1 : player2;
    randomizeLinksForPlayer(p);
}             
