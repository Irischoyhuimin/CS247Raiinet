#include <string>
#include "game.h"
#include "xwindow.h"

class Controller {
    Game game;
    bool setupMode;
    bool textOnly;
    Xwindow* xw;

public:
    Controller(bool textOnly);
    Controller(Xwindow* xw);
    void run();
    void processCommand(const std::string& cmd);
    void parseMove(const std::vector<std::string>& args);
    void parseAbility(const std::vector<std::string>& args);
    void parseSetup(const std::vector<std::string>& args);
    void printHelp();
};