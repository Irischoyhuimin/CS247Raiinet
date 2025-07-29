#ifndef SCAN_H
#define SCAN_H

#include "ability.h"
#include "player.h"
#include <iostream>

class Scan : public Ability {
public:
    Scan() { used = false; }
    void apply(Player& activePlayer, Player& opponentPlayer) override {
        // Reveal all link strengths and statuses
        std::cout << "-- Scan results for " << opponentPlayer.getName() << " --\n";
        for (auto& lptr : opponentPlayer.getLinks()) {
            std::cout << "Link " << lptr->getId()
                      << ": Strength=" << lptr->getStrength()
                      << ", Downloaded=" << (lptr->downloaded() ? "Yes" : "No")
                      << ", Boosted=" << (lptr->hasBoost() ? "Yes" : "No")
                      << "\n";
        }
        markUsed();
    }
    bool isValid(Player& player) const override {
        // Can use if not yet used
        return !used;
    }
    std::string getType() const override { return "Scan"; }
};

#endif // SCAN_H
