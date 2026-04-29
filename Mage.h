#pragma once
#include "Player.h"

class Mage : public Player {
public:
    Mage(const std::string& name, int hp, int mp, int attack, int defense)
        : Player(name, hp, mp, attack, defense) {
        Mp += 50;
        MaxMp += 50;
    }

    void AttackEnemy() override {
        // 마법 공격 구현
    }

    std::string WhoAmI() override {
        return "Mage";
    }
};