#ifndef SWAP_H
#define SWAP_H

#include "ability.h"
#include "player.h"

class Swap : public Ability {
public:
    Swap() { used = false; }
    void apply(Player& player) override {
        // Swap first two links in player's list
        auto& links = player.getLinks();
        if (links.size() >= 2) std::swap(links[0], links[1]);
        markUsed();
    }
    bool isValid(Player& player) const override {
        // Can use if not yet used and at least two links exist
        return !used && player.getLinks().size() >= 2;
    }
    std::string getType() const override { return "Swap"; }
};

#endif // SWAP_H
