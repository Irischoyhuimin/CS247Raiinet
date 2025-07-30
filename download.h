#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "ability.h"
#include "player.h"

class Download : public Ability {
public:
    Download() { used = false; }
    void apply(Player& active, Player& opponent, const vector<string>& args, Board& board) {
        if (args.size() < 1) {
            cout << "Usage: ability <index> <linkId>\n";
            return;
        }

        char targetId = args[0][0];
        Link* target = opponent.getLinkById(targetId);
        if (!target || target->downloaded()) {
            cout << "Invalid target link for download.\n";
            return;
        }

        // Remove from the board
        if (Cell* cell = board.findCellOf(target)) {
            cell->removeLink();             // clears the piece from the grid
        }

        // Mark as downloaded (updates D/V counters)
        opponent.downloadLink(target);

        cout << "Downloaded opponent's link " << targetId << ".\n";
    }
    bool isValid(Player& player) const override {
        // Can use if not yet used and at least one link is available to download
        if (used) return false;
        for (auto& lptr : player.getLinks()) {
            if (!lptr->downloaded()) return true;
        }
        return false;
    }
    string getType() const override { return "Download"; }
};

#endif 
