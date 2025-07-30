#ifndef LINKBOOST_H
#define LINKBOOST_H

#include "ability.h"
#include "player.h"

class LinkBoost : public Ability {
public:
    LinkBoost() { used = false; }
    void apply(Player& active, Player& /*opp*/,
               const std::vector<std::string>& args, Board& /*board*/) override {
        if (used) {
            std::cout << "LinkBoost already used.\n";
            return;
        }
        if (args.empty()) {
            std::cout << "Usage: ability <index> <linkId>  (e.g., ability 1 b)\n";
            return;
        }

        // Find the targeted link owned by the active player
        Link* target = active.getLinkById(args[0][0]);
        if (!target) {
            std::cout << "No such link: " << args[0] << "\n";
            return;
        }
        if (target->downloaded()) {
            std::cout << "Cannot boost a downloaded link.\n";
            return;
        }

        // Apply the boost; your move code must honor hasBoost()
        target->setBoost(true);
        std::cout << "LinkBoost applied to link " << target->getId() << ".\n";
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
