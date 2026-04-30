#pragma once
#include "Player.h"

class Archer : public Player {
public:
    Archer(const std::string& name, int hp, int mp, int attack, int defense)
        : Player(name, hp, mp, attack, defense) {
        attack += 30;
    }

    std::string AttackEnemy() override {
        return std::string("Archer's Attack!");
    }

    std::string WhoAmI() override {
        return "Archer";
    }
};