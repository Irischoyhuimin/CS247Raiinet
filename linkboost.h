#ifndef LINKBOOST_H
#define LINKBOOST_H

#include "ability.h"
#include "player.h"

class LinkBoost : public Ability {
public:
    LinkBoost() { used = false; }
    void apply(Player& player) override {
        // Boost all active (non-downloaded) links
        for (auto& lptr : player.getLinks()) {
            if (!lptr->downloaded()) {
                lptr->setBoost(true);
            }
        }
        markUsed();
    }
    bool isValid(Player& player) const override {
        // Can use if not yet used and at least one link is available
        if (used) return false;
        for (auto& lptr : player.getLinks()) {
            if (!lptr->downloaded()) return true;
        }
        return false;
    }
    std::string getType() const override { return "LinkBoost"; }
};

#endif // LINKBOOST_H
