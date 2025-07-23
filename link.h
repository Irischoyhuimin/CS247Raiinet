#include <string>

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
    virtual std::string getType() const = 0;
    char getId() const;
    int getStrength() const;
    bool hasBoost() const;
    void setBoost(bool boost);
    void setDownloaded(bool downloaded);
    bool downloaded() const;
    Player* getOwner() const;
};

class Virus : public Link {
public:
    Virus(char id, Player* owner, int strength);
    std::string getType() const override;
};

class Data : public Link {
public:
    Data(char id, Player* owner, int strength);
    std::string getType() const override;
};

