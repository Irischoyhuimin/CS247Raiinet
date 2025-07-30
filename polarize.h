#ifndef POLARIZE_H
#define POLARIZE_H

#include "ability.h"
#include "player.h"

class Polarize : public Ability {
public:
    Polarize() { used = false; }
    void apply(Player& active, Player& opponent, const std::vector<std::string>& args, Board& board) {
        if (args.size() < 1) {
            std::cout << "Usage: ability <index> <linkId>\n";
            return;
        }

        char targetId = args[0][0];
        Link* targetLink = opponent.getLinkById(targetId);

        if (!targetLink || targetLink->downloaded()) {
            std::cout << "Invalid target link for polarize.\n";
            return;
        }

        // Flip type but keep same owner/strength/id
        Player* owner = targetLink->getOwner();
        char id = targetLink->getId();
        int strength = targetLink->getStrength();

        // Create the new link
        Link* newLink = nullptr;
        if (targetLink->getType() == "V") {
            newLink = new Data(id, owner, strength);
        } else {
            newLink = new Virus(id, owner, strength);
        }

        // Update Player’s list: replace old link with new link
        owner->replaceLink(targetLink, newLink);

        // Update the Board cell to point to new link
        board.replaceLink(targetLink, newLink);

        std::cout << "Link " << id << " polarized.\n";
    }
    bool isValid(Player& player) const override {
        // Can use if not yet used
        return !used;
    }
    std::string getType() const override { return "Polarize"; }
};

#endif
