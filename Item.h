#pragma once
#include "Configuration.h"

struct Item {
    ItemType Type;
    std::string Name;
    int Price;

    Item() = default;
    Item(ItemType type, int price);

    // 사용 비권장
    void PrintInfo() const;

    // Inventory 템플릿 사용을 위함
    bool operator==(const Item& other) const {
        return this->Name == other.Name;
    }
};