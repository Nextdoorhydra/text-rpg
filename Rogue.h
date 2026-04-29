#pragma once
#include "Player.h"

class Rogue : public Player {
public:
    Rogue(const std::string& name, int hp, int mp, int attack, int defense)
        : Player(name, hp, mp, attack, defense) {
        Attack += 30;
    }

    void AttackEnemy() override {
        // 마법 공격 구현
    }

    std::string WhoAmI() override {
        return "Rogue";
    }
};