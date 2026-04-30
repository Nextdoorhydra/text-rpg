#pragma once
#include "Player.h"

class Thief : public Player {
public:
    Thief(const std::string& name, int hp, int mp, int attack, int defense)
        : Player(name, hp, mp, attack, defense) {
        Attack += 30;
    }

    std::string AttackEnemy() override {
        return std::string("thief's attack!");
    }

    std::string WhoAmI() override {
        return "Rogue";
    }
};