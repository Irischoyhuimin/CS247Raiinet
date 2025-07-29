// ability.cc
#include "ability.h"

// Constructor
Ability::Ability() : used{false} {}

// Mark the ability as used
void Ability::markUsed() {
    used = true;
}

// Return whether ability is used
bool Ability::isUsed() const {
    return used;
}
