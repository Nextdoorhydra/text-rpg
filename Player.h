#pragma once
#include "Character.h"
#include "Inventory.h"
#include "IBattleable.h"

class Player : public Character, public IBattleable {
public:
    Player(const std::string& name, int hp, int mp, int attack, int defense);
    ~Player() {
        // delete Inven; 게임매니저에서 이전 시켜줌
    }

    Inventory* Inven = nullptr;

    void LevelUp() {
        Level++;
        Hp += 10;
        Mp += 10;
        Attack += 3;
        Defense += 1;
        MaxHp = Hp;
        MaxMp = Mp;
    }

    // getter
    int GetLevel() {
        return Level;
    }

    // setter
    void SetLevel(int newLevel) {
        Level = newLevel;
    }

private:
    int Level;    
};

// hardcoding
class NoClassPlayer : public Player {
    public:
    NoClassPlayer(const std::string& name, int hp, int mp, int attack, int defense)
        : Player(name, hp, mp, attack, defense) {
    }
    std::string AttackEnemy() override {
        return std::string("beginner's attack!");
    }

    std::string WhoAmI() override {
        return "Beginner";
    }
};