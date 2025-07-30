#include "player.h"
#include <iostream>
#include "board.h"
#include <sstream>
using namespace std;

Player::Player(const string& name)
  : name{name}
  , dataDownloaded{0}
  , virusDownloaded{0}
  , abilityUsesLeft{5}
{}

void Player::addLink(Link* link) {
    links.emplace_back(link);
}

Link* Player::getLinkById(char id) const {
    for (const auto& lp : links) {
        if (lp->getId() == id && !lp->downloaded()) {
            return lp.get();
        }
    }
    return nullptr;
}

const vector<unique_ptr<Link>>& Player::getLinks() const {
    return links;
}

vector<unique_ptr<Link>>& Player::getLinks() {
    return links;
}

void Player::addAbility(Ability* ability) {
    abilities.emplace_back(move(ability));
}

vector<unique_ptr<Ability>>& Player::getAbilities() {
    return abilities;
}

void Player::useAbility(int abilityIndex, const vector<string>& args, Player& opponent, Board& board) {
    if (abilityIndex < 0 || abilityIndex >= static_cast<int>(abilities.size())) {
        cout << "Invalid ability index\n";
        return;
    }

    auto& ab = abilities[abilityIndex];
    if (ab->isUsed()) {
        cout << "Ability already used\n";
        return;
    }
    if (!ab->isValid(*this)) {
        cout << "Cannot use that ability now\n";
        return;
    }
    bool usedBefore = ab->isUsed();
    ab->apply(*this, opponent, args, board);
    bool usedAfter = ab->isUsed();
    if(!usedBefore && usedAfter){
        --abilityUsesLeft;
    }
}


void Player::downloadLink(Link* link) {
    link->setDownloaded(true);
    if (link->getType() == "D") {
        ++dataDownloaded;
    } else {
        ++virusDownloaded;
    }                         
}

int Player::getDataDownloaded() const  { return dataDownloaded; }
int Player::getVirusDownloaded() const { return virusDownloaded; }

const string& Player::getName() const       { return name; }
int Player::getAbilityUsesLeft() const           { return abilityUsesLeft; }

string Player::getStatusString(const Player& viewer) const {
    stringstream ss;
    ss << name << ":\n";
    ss << "Downloaded: " << dataDownloaded << "D, " << virusDownloaded << "V\n";
    ss << "Abilities: " << abilityUsesLeft << "\n";

    for (const auto& link : links) {
        char id = link->getId();
        ss << id << ": ";
        if (&viewer == this || viewer.isLinkRevealed(id)) {
            ss << link->getType() << link->getStrength();
        } else {
            ss << "?";
        }
        ss << " ";
    }

    ss << "\n";
    return ss.str();
}

void Player::printStatus(const Player& viewer) const {
    cout << getStatusString(viewer);
}

void Player::revealLink(char linkId) {
    revealedOpponentLinks.insert(linkId);
}
    
bool Player::isLinkRevealed(char linkId) const {
    return revealedOpponentLinks.count(linkId) > 0;
}

void Player::concealOpponentLink(char id) { 
    revealedOpponentLinks.erase(id); 
}

void Player::replaceLink(Link* oldL, Link* newL) {
    for (auto& lptr : links) {
        if (lptr.get() == oldL) {
            lptr.reset(newL); 
            return;
        }
    }
}