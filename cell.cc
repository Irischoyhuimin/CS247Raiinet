#include "cell.h"

Cell::Cell(int x, int y)
    : x{x}, y{y}, link{nullptr}, firewallOwner{nullptr}, serverPort{false} {}

Link* Cell::getLink() {
    return link;
}

Link* Cell::getLink() const {
    return link;
}

void Cell::setLink(Link* l) {
    link = l;
    notifyObservers();
}

void Cell::removeLink() {
    link = nullptr;
    notifyObservers();
}

bool Cell::hasFirewall() const {
    return firewallOwner != nullptr;
}

void Cell::setFirewall(Player* owner) {
    firewallOwner = owner;
    notifyObservers();
}

Player* Cell::getFirewallOwner() const {
    return firewallOwner;
}

void Cell::removeFirewall() {
    firewallOwner = nullptr;
    notifyObservers();
}

bool Cell::isOccupied() const {
    return link != nullptr;
}

bool Cell::isServerPort() const {
    return serverPort;
}

void Cell::setServerPort(bool val) {
    serverPort = val;
    notifyObservers();
}

void Cell::attach(Observer* o) {
    observers.push_back(o);
}

void Cell::detach(Observer* o) {
    for (auto it = observers.begin(); it != observers.end(); ++it) {
        if (*it == o) {
            observers.erase(it);
            break;
        }
    }
}


void Cell::notifyObservers() {
    for (auto& o : observers) {
        o->update();
    }
}
