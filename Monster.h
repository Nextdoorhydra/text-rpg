#pragma once
#include "Character.h"
#include "Item.h"
#include "IBattleable.h"

class Monster : public Character, public IBattleable {
public:
    Monster(const std::string& name, int hp, int mp, int attack, int defense, Item item)
        : Character(name, hp, mp, attack, defense), dropItem(item) {
    }

    Item dropItem;
};