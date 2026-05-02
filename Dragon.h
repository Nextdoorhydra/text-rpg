#pragma once
#include "Monster.h"

class Dragon : public Monster {
public:
    Dragon(const std::string& name, int hp, int mp, int attack, int defense, Item item)
        : Monster(name, hp, mp, attack, defense, Item(item)){
    }

    std::string AttackEnemy(Character* enemy) override {
        if (!enemy)
            return std::string("");

        int damage = GetAttack() - enemy->GetDefense() > 0 ? GetAttack() - enemy->GetDefense() : 1;
        damage *= 5;

        enemy->SetHp(enemy->GetHp() - damage);
        enemy->SetHp(enemy->GetHp() - damage);

        return std::string("Dragon B R E A T H");
    }

    std::string WhoAmI() {
        return std::string("Dragon");
    }
};