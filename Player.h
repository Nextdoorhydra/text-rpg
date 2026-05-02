#pragma once
#include "Character.h"
#include "Inventory.h"
#include "IBattleable.h"
#include "Item.h"

class Player : public Character, public IBattleable {
public:
    Player(const std::string& name, int hp, int mp, int attack, int defense);
    ~Player() {
        // delete Inven; 게임매니저에서 이전 시켜줌
    }

    Inventory<Item>* Inven = nullptr;

    int CurrentEXP = 0;
    int MaxEXP = 100;

    void GainExp(int expAmount) {
        CurrentEXP += expAmount;

        while (CurrentEXP >= MaxEXP) {
            CurrentEXP -= MaxEXP;
            LevelUp();
        }
    }

    // getter
    int GetLevel() {
        return Level;
    }

    // setter
    void SetLevel(int newLevel) {
        Level = newLevel;
    }

    // 포인터를 쓰지 않고 매개변수만 넘겼을 때 call by value로 스택에
    // 넘긴 변수가 따로 복사되어 이용되기 때문에 원본에 영향을 못끼친다(도전 step1 q)
    void SetPotion(int count, int* p_HPPotion, int* p_MPPotion) {
        *p_HPPotion = count;
        *p_MPPotion = count;
    }

private:
    int Level;

    void LevelUp() {
        Level++;
        Hp += 10;
        Mp += 10;
        Attack += 3;
        Defense += 1;
        MaxHp = Hp;
        MaxMp = Mp;
    }
};

// hardcoding
class NoClassPlayer : public Player {
    public:
    NoClassPlayer(const std::string& name, int hp, int mp, int attack, int defense)
        : Player(name, hp, mp, attack, defense) {
    }

    std::string AttackEnemy(Character* enemy) override {
        if (!enemy)
            return std::string("");

        int damage = GetAttack() - enemy->GetDefense() > 0 ? GetAttack() - enemy->GetDefense() : 1;

        enemy->SetHp(enemy->GetHp() - damage);

        return std::string("Beginer's Attack!");
    }

    std::string WhoAmI() override {
        return "Beginner";
    }
};