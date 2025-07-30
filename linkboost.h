#ifndef LINKBOOST_H
#define LINKBOOST_H

#include "ability.h"
#include "player.h"

class LinkBoost : public Ability {
public:
    LinkBoost() { used = false; }
    void apply(Player& active, Player& /*opp*/,
               const vector<string>& args, Board& /*board*/) override {
        if (used) {
            cout << "LinkBoost already used.\n";
            return;
        }
        if (args.empty()) {
            cout << "Usage: ability <index> <linkId>  (e.g., ability 1 b)\n";
            return;
        }

        // Find the targeted link owned by the active player
        Link* target = active.getLinkById(args[0][0]);
        if (!target) {
            cout << "No such link: " << args[0] << "\n";
            return;
        }
        if (target->downloaded()) {
            cout << "Cannot boost a downloaded link.\n";
            return;
        }

        // Apply the boost
        target->setBoost(true);
        markUsed();
        cout << "LinkBoost applied to link " << target->getId() << ".\n";
    }
    bool isValid(Player& player) const override {
        // Can use if not yet used and at least one link is available
        if (used) return false;
        for (auto& lptr : player.getLinks()) {
            if (!lptr->downloaded()) return true;
        }
        return false;
    }
    string getType() const override { return "LinkBoost"; }
};

#endif 
