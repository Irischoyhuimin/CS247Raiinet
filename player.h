#pragma once

#include <memory>
#include <string>
#include <vector>
#include <set>
#include "link.h"
#include "ability.h"
class Board; 
class Player {
    string name;
    vector<unique_ptr<Link>>    links;
    vector<unique_ptr<Ability>> abilities;
    set<char> revealedOpponentLinks;
    int dataDownloaded    = 0;
    int virusDownloaded   = 0;
    int abilityUsesLeft   = 5;

public:
    explicit Player(const string& name);

    //  Link management 
    void addLink(Link* link);
    Link* getLinkById(char id) const;
    const vector<unique_ptr<Link>>& getLinks() const;
    vector<unique_ptr<Link>>& getLinks();

    //  Ability management 
    void addAbility(Ability* ability);
    vector<unique_ptr<Ability>>& getAbilities();
    void useAbility(int abilityIndex, const vector<string>& args, Player& opponent, Board& board);

    //  Download tracking 
    void downloadLink(Link* link);
    int getDataDownloaded()  const;
    int getVirusDownloaded() const;

    //  Misc 
    const string& getName() const;
    int getAbilityUsesLeft() const;
    void printStatus(const Player& viewer) const;
    void revealLink(char linkId);
    bool isLinkRevealed(char linkId) const;
    void concealOpponentLink(char id);
    void replaceLink(Link* oldL, Link* newL);
    void setAbilityUsesLeft(int n) { abilityUsesLeft = n; }
    string getStatusString(const Player& viewer) const;
};
