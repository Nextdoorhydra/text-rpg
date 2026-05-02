#pragma once
#include "Player.h"

class Mage : public Player {
public:
    Mage(const std::string& name, int hp, int mp, int attack, int defense)
        : Player(name, hp, mp, attack, defense) {
        Mp += 50;
        MaxMp += 50;
    }

    std::string AttackEnemy(Character* enemy) override {
        if (!enemy)
            return std::string("");

        int damage = GetAttack() - enemy->GetDefense() > 0 ? GetAttack() - enemy->GetDefense() : 1;
        damage *= 2;

        enemy->SetHp(enemy->GetHp() - damage);

        return std::string("Mage's FireBall Attack!");
    }

    std::string WhoAmI() override {
        return "Mage";
    }
};