#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "ability.h"
#include "player.h"

class Download : public Ability {
public:
    Download() { used = false; }
    void apply(Player& activePlayer, Player& opponentPlayer) override {
        // Download the first available non-downloaded link
        for (auto& lptr : activePlayer.getLinks()) {
            if (!lptr->downloaded()) {
                activePlayer.downloadLink(lptr.get());
                break;
            }
        }
        markUsed();
    }
    bool isValid(Player& player) const override {
        // Can use if not yet used and at least one link is available to download
        if (used) return false;
        for (auto& lptr : player.getLinks()) {
            if (!lptr->downloaded()) return true;
        }
        return false;
    }
    std::string getType() const override { return "Download"; }
};

#endif // DOWNLOAD_H
