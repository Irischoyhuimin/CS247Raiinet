#ifndef LINK_H
#define LINK_H
#include <string>
using namespace std;

class Player;

class Link {
protected:
    char id;
    Player* owner;
    int strength;
    bool isDownloaded;
    bool isBoosted;

public:
    Link(char id, Player* owner, int strength);
    virtual ~Link() = default;
    virtual string getType() const = 0;
    char getId() const;
    Player* getOwner() const;
    int getStrength() const;
    bool downloaded() const;
    void setDownloaded(bool val);
    bool hasBoost() const;
    void setBoost(bool boost);
};

class Virus : public Link {
public:
    Virus(char id, Player* owner, int strength);
    string getType() const override;
};

class Data : public Link {
public:
    Data(char id, Player* owner, int strength);
    string getType() const override;
};
#endif