#pragma once
#include "Character.h"
#include "Inventory.h"

class Player : public Character {
public:
    Player(const std::string& name, int hp, int mp, int attack, int defense);
    ~Player() {
        // delete Inven; 게임매니저에서 이전 시켜줌
    }

    Inventory* Inven = nullptr;

    virtual void AttackEnemy() = 0;

    virtual std::string WhoAmI() = 0;


    void LevelUp() {
        Level++;
        Hp += 10;
        Mp += 10;
        Attack += 5;
        Defense += 5;
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
    void AttackEnemy() override {

    }

    std::string WhoAmI() override {
        return "Beginner";
    }
};