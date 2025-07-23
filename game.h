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
    void start();
    void init();
    void toggleEnhancements();
    void move(std::string linkName, std::string dir);
    void useAbility(std::string ability, const std::vector<std::string>& args);
    void display() const;
    bool isGameOver();
    void switchPlayers();
};
