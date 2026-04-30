#pragma once
#include "Player.h"

class Mage : public Player {
public:
    Mage(const std::string& name, int hp, int mp, int attack, int defense)
        : Player(name, hp, mp, attack, defense) {
        Mp += 50;
        MaxMp += 50;
    }

    std::string AttackEnemy() override {
        return std::string("Mage's Attack!");
    }

    std::string WhoAmI() override {
        return "Mage";
    }
};