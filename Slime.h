#pragma once
#include "Monster.h"

class Slime : public Monster {
public:
    Slime(const std::string& name, int hp, int mp, int attack, int defense, Item item)
        : Monster(name, hp, mp, attack, defense, Item(item)){
    }

    std::string AttackEnemy(Character* enemy) override {
        if (!enemy)
            return std::string("");

        int damage = GetAttack() - enemy->GetDefense() > 0 ? GetAttack() - enemy->GetDefense() : 1;

        enemy->SetHp(enemy->GetHp() - damage);

        return std::string("Slime's Attack!");
    }

    std::string WhoAmI() {
        return std::string("Slime");
    }
};