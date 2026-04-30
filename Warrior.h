#pragma once
#include "Player.h"

class Warrior : public Player {
public:
    Warrior(const std::string& name, int hp, int mp, int attack, int defense)
        : Player(name, hp, mp, attack, defense) {
        Hp += 30;
        MaxHp += 30;
    }

    std::string AttackEnemy() override {
        return std::string("Warrior's Attack!");
    }

    std::string WhoAmI() override {
        return "Warrior";
    }
};