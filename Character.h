#pragma once
#include <string>

class Character {
public:
    Character(const std::string& name, int hp, int mp, int attack, int defense)
        : Name(name), Hp(hp), Mp(mp), Attack(attack), Defense(defense), MaxHp(hp), MaxMp(mp) {
    }

    virtual ~Character() {}

    std::string Name;
    int Hp;
    int MaxHp;
    int Mp;
    int MaxMp;
    int Attack;
    int Defense;
};
