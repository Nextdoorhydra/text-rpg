#pragma once
#include <string>

class IBattleable {
public:
    virtual std::string AttackEnemy() = 0;
};