#ifndef HEAL_H
#define HEAL_H

#include "ability.h"
#include "player.h"

class Heal : public Ability {
public:
    Heal() { used = false; }
    void apply(Player& activePlayer, Player& opponentPlayer) override {
        // Restore the first downloaded link
        for (auto& lptr : activePlayer.getLinks()) {
            if (lptr->downloaded()) {
                lptr->setDownloaded(false);
                break;
            }
        }
        markUsed();
    }
    bool isValid(Player& player) const override {
        // Can use if not yet used and at least one link is downloaded
        if (used) return false;
        for (auto& lptr : player.getLinks()) {
            if (lptr->downloaded()) return true;
        }
        return false;
    }
    std::string getType() const override { return "Heal"; }
};

#endif 
