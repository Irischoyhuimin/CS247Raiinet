#ifndef SWAP_H
#define SWAP_H

#include "ability.h"
#include "player.h"

class Swap : public Ability {
public:
    Swap() { used = false; }
    void apply(Player& active, Player& opp, const std::vector<std::string>& args, Board& board) {
        if (args.size() < 2) {
            std::cout << "Usage: ability <index> <link1> <link2>\n";
            return;
        }
        Link* l1 = active.getLinkById(args[0][0]);
        Link* l2 = active.getLinkById(args[1][0]);
        if (!l1 || !l2 || l1->downloaded() || l2->downloaded()) {
            std::cout << "Invalid links to swap.\n";
            return;
        }
        if (board.swapLinks(l1, l2)) {
            std::cout << "Swapped " << l1->getId() << " and " << l2->getId() << ".\n";
        } else {
            std::cout << "Swap failed: could not find links on board.\n";
        }
    }
    bool isValid(Player& player) const override {
        if (used) return false;
        int count = 0;
        for (auto& l : player.getLinks()) {
            if (!l->downloaded()) count++;
            if (count >= 2) return true;
        }
        return false;
    }
    std::string getType() const override { return "Swap"; }
};

#endif // SWAP_H
