#ifndef FIREWALL_H
#define FIREWALL_H

#include "ability.h"
#include "player.h"

class Firewall : public Ability {
public:
    Firewall() { used = false; }
    void apply(Player& active, Player& /*opp*/, const vector<string>& args, Board& board) override {
        if (used) {
            cout << "Firewall already used.\n";
            return;
        }
        // Expect: ability <index> <col> <row>
        if (args.size() < 2) {
            cout << "Usage: ability <index> <col> <row>\n";
            return;
        }

        int col = -1, row = -1;
        try {
            col = stoi(args[0]);   // horizontal (x)
            row = stoi(args[1]);   // vertical   (y)
        } catch (...) {
            cout << "Column/row must be integers.\n";
            return;
        }

        // Board methods are (row, col) -> swap when calling
        if (!board.canPlaceFirewallAt(row, col)) {
            cout << "Cannot place firewall at (" << col << "," << row << ").\n";
            return;
        }

        board.placeFirewall(row, col, &active);
        markUsed();
        cout << "Firewall placed at (" << col << "," << row << ").\n";
    }
    bool isValid(Player& player) const override { return !used; }
    string getType() const override { return "Firewall"; }
};

#endif
