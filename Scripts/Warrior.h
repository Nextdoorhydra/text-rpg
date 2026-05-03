#pragma once
#include "Player.h"

class Warrior : public Player {
public:
    Warrior(const std::string& name, int hp, int mp, int attack, int defense)
        : Player(name, hp, mp, attack, defense) {
        Hp += 30;
        MaxHp += 30;
    }

    std::string AttackEnemy(Character* enemy) override {
        if (!enemy)
            return std::string("");

        int damage = GetAttack() - enemy->GetDefense() > 0 ? GetAttack() - enemy->GetDefense() : 1;
        damage *= 3;

        SetHp(GetHp() - GetHp() / 10); // 10ÆÛ ÀÚÇØ
        enemy->SetHp(enemy->GetHp() - damage);

        return std::string("Warrior's furious Attack!");
    }

    std::string WhoAmI() override {
        return "Warrior";
    }
};