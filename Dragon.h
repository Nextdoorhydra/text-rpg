#pragma once
#include "Monster.h"

class Dragon : public Monster {
public:
    Dragon(const std::string& name, int hp, int mp, int attack, int defense, Item item)
        : Monster(name, hp, mp, attack, defense, Item(item)){
    }
    std::string AttackEnemy() {
        return std::string("Dragon's attack!");
    }

    std::string WhoAmI() {
        return std::string("Slime");
    }
};