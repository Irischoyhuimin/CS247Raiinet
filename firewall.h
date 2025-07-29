#ifndef FIREWALL_H
#define FIREWALL_H

#include "ability.h"
#include "player.h"

class Firewall : public Ability {
public:
    Firewall() { used = false; }
    void apply(Player& player) override {
        // Apply a protective firewall: prevent next battle losses on all links
        for (auto& lptr : player.getLinks()) {
            // Use the boost flag as a placeholder for firewall protection
            if (!lptr->downloaded()) {
                lptr->setBoost(true);
            }
        }
        markUsed();
    }
    bool isValid(Player& player) const override {
        // Can use if not yet used
        return !used;
    }
    std::string getType() const override { return "Firewall"; }
};

#endif // FIREWALL_H
