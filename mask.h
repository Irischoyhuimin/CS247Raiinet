#ifndef MASK_H
#define MASK_H

#include "ability.h"
#include "player.h"
#include <iostream>

class MaskAbility : public Ability {
public:
    MaskAbility() { used = false; }
    void apply(Player& active, Player& opponent, const vector<string>& args, Board& /*board*/) override {
        if (args.size() < 1) {
            cout << "Usage: ability <index> <linkId>\n";
            return;
        }

        char id = args[0][0];
        Link* myLink = active.getLinkById(id);
        if (!myLink || myLink->downloaded()) {
            cout << "Invalid link to mask.\n";
            return;
        }
        opponent.concealOpponentLink(id);

        cout << "Masked link " << id << " (opponent now sees '?').\n";
    }
    bool isValid(Player& player) const override {
        // Can use if not yet used
        return !used;
    }
    string getType() const override { return "Mask"; }
};

#endif // MASK_H
