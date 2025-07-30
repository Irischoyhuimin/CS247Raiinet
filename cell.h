#ifndef CELL_H
#define CELL_H
#include <vector>
#include "observer.h"
#include "link.h"

class Player;

class Cell {
    int x, y;
    Link* link;
    Player* firewallOwner;
    bool serverPort;
    vector<Observer*> observers;

public:
    Cell(int x, int y);
    Link* getLink();
    Link* getLink() const;
    void setLink(Link* l);
    void removeLink();
    bool hasFirewall() const;
    void setFirewall(Player* owner);
    Player* getFirewallOwner() const;
    void removeFirewall();
    bool isOccupied() const;
    bool isServerPort() const;
    void setServerPort(bool val);
    void attach(Observer* o);
    void detach(Observer* o);
    void notifyObservers();
};
#endif
