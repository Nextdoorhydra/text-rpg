#include "Item.h"
#include "Configuration.h"

Item::Item(ItemType type, int price) {
    this->Type = type;
    this->Price = price;

    switch (type) {
    case ItemType::HPPotion:
        this->Name = "HPPotion";
        break;
    case ItemType::MPPotion:
        this->Name = "MPPotion";
        break;
    case ItemType::Sword:
        this->Name = "Sword";
        break;
    case ItemType::Shield:
        this->Name = "Shield";
        break;
    case ItemType::GoblinEar:
        this->Name = "GoblinEar";
        break;
    case ItemType::SlimeJelly:
        this->Name = "SlimeJelly";
        break;
    case ItemType::DragonHorn:
        this->Name = "DragonHorn";
        break;
    case ItemType::OrcHead:
        this->Name = "OrcHead";
        break;
    case ItemType::EmptyBottle:
        this->Name = "EmptyBottle";
        break;
    }
}

void Item::PrintInfo() const {

}