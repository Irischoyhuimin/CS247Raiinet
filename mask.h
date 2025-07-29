#ifndef MASK_H
#define MASK_H

#include "ability.h"
#include "player.h"
#include <iostream>

class MaskAbility : public Ability {
public:
    MaskAbility() { used = false; }
    void apply(Player& activePlayer, Player& opponentPlayer) override {
        // Conceal link IDs until next scan
        std::cout << "Mask applied: link IDs hidden until next scan.\n";
        for (auto& lptr : activePlayer.getLinks()) {
            lptr->setBoost(false); // use boost flag as mask placeholder
        }
        markUsed();
    }
    bool isValid(Player& player) const override {
        // Can use if not yet used
        return !used;
    }
    std::string getType() const override { return "Mask"; }
};

#endif // MASK_H
