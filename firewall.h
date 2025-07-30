#ifndef FIREWALL_H
#define FIREWALL_H

#include "ability.h"
#include "player.h"

class Firewall : public Ability {
public:
    Firewall() { used = false; }
    void apply(Player& active, Player& /*opp*/,
               const std::vector<std::string>& args, Board& board) override {
        if (used) {
            std::cout << "Firewall already used.\n";
            return;
        }
        // Expect: ability <index> <col> <row>
        if (args.size() < 2) {
            std::cout << "Usage: ability <index> <col> <row>\n";
            return;
        }

        int col = -1, row = -1;
        try {
            col = std::stoi(args[0]);   // horizontal (x)
            row = std::stoi(args[1]);   // vertical   (y)
        } catch (...) {
            std::cout << "Column/row must be integers.\n";
            return;
        }

        // Board methods are (row, col) -> swap when calling
        if (!board.canPlaceFirewallAt(row, col)) {
            std::cout << "Cannot place firewall at (" << col << "," << row << ").\n";
            return;
        }

        board.placeFirewall(row, col, &active);
        std::cout << "Firewall placed at (" << col << "," << row << ").\n";
    }
    bool isValid(Player& player) const override { return !used; }
    std::string getType() const override { return "Firewall"; }
};

#endif // FIREWALL_H
