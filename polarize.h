#ifndef POLARIZE_H
#define POLARIZE_H

#include "ability.h"
#include "player.h"

class Polarize : public Ability {
public:
    Polarize() { used = false; }
    void apply(Player& activePlayer, Player& opponentPlayer) override {
        // Invert boost state of all links
        for (auto& lptr : activePlayer.getLinks()) {
            lptr->setBoost(!lptr->hasBoost());
        }
        markUsed();
    }
    bool isValid(Player& player) const override {
        // Can use if not yet used
        return !used;
    }
    std::string getType() const override { return "Polarize"; }
};

#endif // POLARIZE_H
