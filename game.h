#pragma once

#include <string>
#include <vector>
#include "board.h"
#include "player.h"
#include "xwindow.h"
//our Command Interpreter 
class Game
{
    Board board;
    Player player1;
    Player player2;
    Player *activePlayer; 
    Player *inactivePlayer;
    bool enhancementsOn;
    bool textOnly;
    Xwindow *xw;

public:
    Game(bool textOnly);
    Game(Xwindow *xw);

    void init();
    void start();
    void toggleEnhancements();

    void move(const string &dir, const string &linkId);
    void useAbility(const string &abilityIdx, const vector<string> &args);
    void display() const;
    bool isGameOver() const;
    int getWinner() const;
    void switchPlayers();
    Player &getCurrentPlayer();
    const Player &getCurrentPlayer() const;
    void loadLinkConfig(int playerNum, const string &filename);
    void setAbilityOrder(int playerNum, const vector<string> &order);
    void randomizeLinksForPlayer(Player& player);
    void simpleRandomizeLinks(vector<unique_ptr<Link>>& links);
    void randomizeLinksForPlayer(int playerNum);
};
