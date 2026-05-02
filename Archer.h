#pragma once
#include "Player.h"

class Archer : public Player {
public:
    Archer(const std::string& name, int hp, int mp, int attack, int defense)
        : Player(name, hp, mp, attack, defense) {
        attack += 30;
    }

    std::string AttackEnemy(Character* enemy) override {        
        if (!enemy)
            return std::string("");

        int damage = GetAttack() - enemy->GetDefense() > 0 ? GetAttack() - enemy->GetDefense() : 1;
        damage = damage / 3 > 0 ? damage / 3 : 1;

        enemy->SetHp(enemy->GetHp() - damage);
        enemy->SetHp(enemy->GetHp() - damage);
        enemy->SetHp(enemy->GetHp() - damage);

        return std::string("Archer's 3-Chain Attack!");
    }

    std::string WhoAmI() override {
        return "Archer";
    }
};