#pragma once

#include <memory>
#include <string>
#include <vector>
#include <set>
#include "link.h"
#include "ability.h"
class Board; 
class Player {
    std::string name;
    std::vector<std::unique_ptr<Link>>    links;
    std::vector<std::unique_ptr<Ability>> abilities;
    std::set<char> revealedOpponentLinks;
    int dataDownloaded    = 0;
    int virusDownloaded   = 0;
    int abilityUsesLeft   = 5;

public:
    explicit Player(const std::string& name);
    void addLink(Link* link);
    Link* getLinkById(char id) const;
    const std::vector<std::unique_ptr<Link>>& getLinks() const;
    std::vector<std::unique_ptr<Link>>& getLinks();
    void addAbility(std::unique_ptr<Ability> ability);
    std::vector<std::unique_ptr<Ability>>& getAbilities();
    void useAbility(int abilityIndex, const std::vector<std::string>& args, Player& opponent);
    void downloadLink(Link* link);
    int  getDataDownloaded()  const;
    int  getVirusDownloaded() const;
    const std::string& getName() const;
    int getAbilityUsesLeft() const;
    void printStatus(const Player& viewer) const;
    void revealLink(char linkId);
    bool isLinkRevealed(char linkId) const;
    void concealOpponentLink(char id);
    void replaceLink(Link* oldL, Link* newL);
    void setAbilityUsesLeft(int n) { abilityUsesLeft = n; }
    string getStatusString(const Player& viewer) const;
};
