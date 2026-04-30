#pragma once
#include "Monster.h"

class Goblin : public Monster {
public:
    Goblin(const std::string& name, int hp, int mp, int attack, int defense, Item item)
        : Monster(name, hp, mp, attack, defense, Item(item)) {
    }
    std::string AttackEnemy() {
        return std::string("Goblin's attack!");
    }

    std::string WhoAmI() {
        return std::string("Slime");
    }
};