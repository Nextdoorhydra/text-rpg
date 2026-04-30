#include "Item.h"
#include "Configuration.h"

Item::Item(ItemType type, int price) {
    this->Type = type;
    this->Price = price;

    switch (type) {
    case ItemType::HPPotion:
        this->Name = "HP Potion";
        break;
    case ItemType::MPPotion:
        this->Name = "MP Potion";
        break;
    case ItemType::Sword:
        this->Name = "Sword";
        break;
    case ItemType::Shield:
        this->Name = "Shield";
        break;
    case ItemType::GoblinEar:
        this->Name = "Goblin Ear";
        break;
    case ItemType::SlimeJelly:
        this->Name = "Slime Jelly";
        break;
    case ItemType::DragonHorn:
        this->Name = "Dragon Horn";
        break;
    case ItemType::OrcHead:
        this->Name = "Orc Head";
        break;
    }
}

bool Item::operator<(const Item& other) const {
    return this->Name < other.Name;
}

void Item::PrintInfo() const {

}