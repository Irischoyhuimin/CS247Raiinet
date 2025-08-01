#ifndef SWAP_H
#define SWAP_H

#include "ability.h"
#include "player.h"

class Swap : public Ability {
public:
    Swap() { used = false; }
    void apply(Player& active, Player& opp, const vector<string>& args, Board& board) {
        if (args.size() < 2) {
            cout << "Usage: ability <index> <link1> <link2>\n";
            return;
        }
        Link* l1 = active.getLinkById(args[0][0]);
        Link* l2 = active.getLinkById(args[1][0]);
        if (!l1 || !l2 || l1->downloaded() || l2->downloaded()) {
            cout << "Invalid links to swap.\n";
            return;
        }
        if (board.swapLinks(l1, l2)) {
            cout << "Swapped " << l1->getId() << " and " << l2->getId() << ".\n";
            markUsed();
        } else {
            cout << "Swap failed: could not find links on board.\n";
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
    string getType() const override { return "Swap"; }
};

#endif 
