#include "link.h"
#include "player.h"
using namespace std;

Link::Link(char id, Player* owner, int strength)
    : id{id}, owner{owner}, strength{strength}, isDownloaded{false}, isBoosted{false} {}

char Link::getId() const {
    return id;
}

Player* Link::getOwner() const {
    return owner;
}

int Link::getStrength() const {
    return strength;
}

bool Link::downloaded() const {
    return isDownloaded;
}

void Link::setDownloaded(bool val) {
    isDownloaded = val;
}

bool Link::hasBoost() const {
    return isBoosted;
}

void Link::setBoost(bool boost) {
    isBoosted = boost;
}

Virus::Virus(char id, Player* owner, int strength)
    : Link{id, owner, strength} {}

string Virus::getType() const {
    return "V";
}

Data::Data(char id, Player* owner, int strength)
    : Link{id, owner, strength} {}

string Data::getType() const {
    return "D";
}
