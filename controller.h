#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>
#include <vector>
#include "game.h"
#include "xwindow.h"

class Controller {
    Game game;
    bool setupMode;
    bool textOnly;
    Xwindow* xw;
    bool abilityUsedThisTurn;
    bool gameOver = false;
    bool link1Loaded = false;
    bool link2Loaded = false;

public:
    Controller(bool textOnly);
    Controller(Xwindow* xw);
    void run();
    void processCommand(const std::string& cmd);
    void parseMove(const std::vector<std::string>& args);
    void parseAbility(const std::vector<std::string>& args);
    void printAbilities();
    void parseSequence(const std::vector<std::string>& args);
    void parseSetup(const std::vector<std::string>& args);
    void printHelp();
    void startGameIfReady();
};

#endif 
