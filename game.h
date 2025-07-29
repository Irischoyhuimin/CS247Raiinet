#pragma once

#include <string>
#include <vector>
#include "board.h"
#include "player.h"
#include "xwindow.h"

class Game {
    Board board;
    Player player1;
    Player player2;
    Player* activePlayer;
    Player* inactivePlayer;
    bool enhancementsOn;
    bool textOnly;
    Xwindow* xw;

public:
    Game(bool textOnly);
    Game(Xwindow* xw);

    void init();
    void start();
    void toggleEnhancements();

    void move(const std::string& dir, const std::string& linkId);
    void useAbility(const std::string& abilityIdx, const std::vector<std::string>& args);
    void display() const;
    bool isGameOver() const;
    void switchPlayers();

    void loadLinkConfig(int playerNum, const std::string& filename);
    void setAbilityOrder(int playerNum, const std::vector<std::string>& order);
};
