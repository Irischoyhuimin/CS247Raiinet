#include <vector>
#include "cell.h"
#include "link.h"
#include "player.h"
#include "display.h"

class Board {
    static const int size = 8;
    std::vector<std::vector<Cell>> grid;
    TextDisplay* td;
    GraphicsDisplay* gd;

public:
    Board();
    Board(Xwindow& xw);
    bool isInvalidMove(const Link& link, int x, int y, const Player& player) const;
    void move(Player* activePlayer, Player* inactivePlayer, Link& link, int x, int y);
    void battle(Player& p1, Player& p2, Link& l1, Link& l2);
    void setup();
    void setupLinks(Player& player, const std::string& config);
    void toggleEnhancementsOn();
    void printTextDisplay() const;
};
