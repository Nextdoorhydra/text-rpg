#include "Inventory.h"
#include "Player.h"

Player::Player(const std::string& name, int hp, int mp, int attack, int defense)
    : Character(name, hp, mp, attack, defense), Level(1) {
    Inven = new Inventory<Item>();
}