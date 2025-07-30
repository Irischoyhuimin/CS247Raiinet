#ifndef ABILITY_H
#define ABILITY_H
#include <string>
#include <vector>
using namespace std;
class Player;
class Board;
class Ability {
protected:
    bool used;

public:
    Ability();
    virtual ~Ability() = default;
    virtual void apply(Player& active, Player& opp, const vector<string>& args, Board& board) = 0;
    virtual bool isValid(Player& player) const = 0;
    bool isUsed() const;
    void markUsed();
    virtual string getType() const = 0;
};

// Subclasses: LinkBoost, Firewall, Download, Scan, Polarize, Heal, Mask, Swap
// Each implements apply() and isValid()
#endif 
