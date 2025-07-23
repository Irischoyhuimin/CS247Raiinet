#include <string>

class Player;

class Ability {
protected:
    bool used;

public:
    Ability();
    virtual void apply(Player& player) = 0;
    virtual bool isValid(Player& player) const = 0;
    bool isUsed() const;
    void markUsed();
    virtual std::string getType() const = 0;
};

// Subclasses: LinkBoost, Firewall, Download, Scan, Polarize, Heal, Mask, Swap
// Each implements apply() and isValid()
