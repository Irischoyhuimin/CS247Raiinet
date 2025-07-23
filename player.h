#include <memory>
#include <string>
#include <vector>
#include "board.h"
#include "display.h"
#include "link.h"
#include "ability.h"

class Player {
    std::unique_ptr<TextDisplay> td;
    std::unique_ptr<GraphicsDisplay> gd;
    std::vector<std::unique_ptr<Link>> links;
    std::vector<std::unique_ptr<Ability>> abilities;
    int dataDownloaded;
    int virusDownloaded;
    int turnCount;
    bool gameOver;
    std::string name;

public:
    Player(std::string name);
    void move(std::string from, std::string dir);
    void useAbility(std::string abilityType, const std::vector<std::string>& args);
    void notifyDisplays();
    void printStatus();
    void setupLinks(const std::string& linkConfig);
};
