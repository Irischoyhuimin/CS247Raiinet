// File: player.cc
#include "player.h"
#include <iostream>

Player::Player(const std::string& name)
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

const std::vector<std::unique_ptr<Link>>& Player::getLinks() const {
    return links;
}

std::vector<std::unique_ptr<Link>>& Player::getLinks() {
    return links;
}

void Player::addAbility(std::unique_ptr<Ability> ability) {
    abilities.emplace_back(std::move(ability));
}

std::vector<std::unique_ptr<Ability>>& Player::getAbilities() {
    return abilities;
}

void Player::useAbility(int abilityIndex, const std::vector<std::string>& args, Player& opponent) {
    if (abilityIndex < 0 ||
        abilityIndex >= static_cast<int>(abilities.size())) {
        std::cout << "Invalid ability index\n";
        return;
    }
    auto& ab = abilities[abilityIndex];
    if (ab->isUsed()) {
        std::cout << "Ability already used\n";
        return;
    }
    if (!ab->isValid(*this)) {
        std::cout << "Cannot use that ability now\n";
        return;
    }
    ab->apply(*this, opponent);
    ab->markUsed();
    --abilityUsesLeft;
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

const std::string& Player::getName() const       { return name; }
int Player::getAbilityUsesLeft() const           { return abilityUsesLeft; }

void Player::printStatus(const Player& viewer) const {
    std::cout << name << ":\n";
    std::cout << "Downloaded: " << dataDownloaded << "D, " 
              << virusDownloaded << "V\n";
    std::cout << "Abilities: " << abilityUsesLeft << "\n";

    for (const auto& link : links) {
        char id = link->getId();
        std::cout << id << ": ";

        if (&viewer == this) {
            std::cout << link->getType() << link->getStrength();
        } else {
            if (viewer.isLinkRevealed(id)) {
                std::cout << link->getType() << link->getStrength();
            } else {
                std::cout << "?";
            }
        }

        std::cout << " ";
    }

    std::cout << "\n";
}

void Player::revealLink(char linkId) {
    revealedOpponentLinks.insert(linkId);
}
    
bool Player::isLinkRevealed(char linkId) const {
    return revealedOpponentLinks.count(linkId) > 0;
}
