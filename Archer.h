#pragma once
#include "Player.h"

class Archer : public Player {
public:
    Archer(const std::string& name, int hp, int mp, int attack, int defense)
        : Player(name, hp, mp, attack, defense) {
        attack += 30;
    }

    void AttackEnemy() override {
        // 마법 공격 구현
    }

    std::string WhoAmI() override {
        return "Archer";
    }
};