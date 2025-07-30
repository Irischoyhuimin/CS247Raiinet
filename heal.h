#ifndef HEAL_H
#define HEAL_H

#include "ability.h"
#include "player.h"

class Heal : public Ability {
public:
    Heal() { used = false; }
    void apply(Player& active, Player& opponent, const vector<string>& args, Board& board) override {
        // Pick target link: by id if provided, else first downloaded link
        Link* toHeal = nullptr;

        if (!args.empty()) {
            char id = args[0][0];
            for (auto& l : active.getLinks()) {
                if (l->getId() == id && l->downloaded()) { toHeal = l.get(); break; }
            }
            if (!toHeal) {
                cout << "No downloaded link with id " << id << " to heal.\n";
                return;
            }
        } else {
            for (auto& l : active.getLinks()) {
                if (l->downloaded()) { toHeal = l.get(); break; }
            }
            if (!toHeal) {
                cout << "No downloaded links to heal.\n";
                return;
            }
        }

        int r, c;
        if (!board.findRespawnSquare(&active, r, c, &opponent)) {
            cout << "No safe square available to respawn.\n";
            return;
        }

        board.placeLink(toHeal, r, c);
        markUsed();
        cout << "Healed link " << toHeal->getId() << " at (" << c << "," << r << ").\n";
    }
    bool isValid(Player& player) const override {
        // Can use if not yet used and at least one link is downloaded
        if (used) return false;
        for (auto& lptr : player.getLinks()) {
            if (lptr->downloaded()) return true;
        }
        return false;
    }
    string getType() const override { return "Heal"; }
};

#endif 
