#pragma once

#include <memory>
#include <string>
#include <vector>

#include "link.h"
#include "ability.h"

class Player {
    std::string name;
    std::vector<std::unique_ptr<Link>>    links;
    std::vector<std::unique_ptr<Ability>> abilities;
    int dataDownloaded    = 0;
    int virusDownloaded   = 0;
    int abilityUsesLeft   = 5;

public:
    explicit Player(const std::string& name);

    // --- Link management ---
    void addLink(Link* link);
    Link* getLinkById(char id) const;
    const std::vector<std::unique_ptr<Link>>& getLinks() const;

    // --- Ability management ---
    void addAbility(std::unique_ptr<Ability> ability);
    std::vector<std::unique_ptr<Ability>>& getAbilities();
    void useAbility(int abilityIndex, const std::vector<std::string>& args);

    // --- Download tracking ---
    void downloadLink(Link* link);
    int  getDataDownloaded()  const;
    int  getVirusDownloaded() const;

    // --- Misc ---
    const std::string& getName() const;
    int getAbilityUsesLeft() const;
    void printStatus() const;
};