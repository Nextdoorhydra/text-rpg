#pragma once
#include "Player.h"

class Thief : public Player {
public:
    Thief(const std::string& name, int hp, int mp, int attack, int defense)
        : Player(name, hp, mp, attack, defense) {
        Attack += 30;
    }

    std::string AttackEnemy(Character* enemy) override {
        if (!enemy)
            return std::string("");

        int damage = GetAttack() - enemy->GetDefense() > 0 ? GetAttack() - enemy->GetDefense() : 1;
        damage = damage / 5 > 0 ? damage / 5 : 1;

        enemy->SetHp(enemy->GetHp() - damage);
        enemy->SetHp(enemy->GetHp() - damage);
        enemy->SetHp(enemy->GetHp() - damage);
        enemy->SetHp(enemy->GetHp() - damage);
        enemy->SetHp(enemy->GetHp() - damage);

        return std::string("Thief's 5-chain Attack!");
    }

    std::string WhoAmI() override {
        return "Rogue";
    }
};