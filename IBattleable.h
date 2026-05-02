#pragma once
#include <string>

class IBattleable {
public:
    virtual std::string AttackEnemy(Character* s) = 0;
};