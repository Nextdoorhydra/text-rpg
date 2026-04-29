#pragma once
#include <map>
#include "Configuration.h"

class Inventory {
public:
    std::map<ItemType, int> Items;


    bool TryUseItem(ItemType itemType) {
        if (Items.count(itemType) > 0 && Items[itemType] > 0) {

            RemoveItem(itemType);
            return true;
        }

        return false;
    }

    // 아이템 추가
    void AddItem(ItemType itemType) {
        Items[itemType]++;
    }
    // 아이템 제거
    void RemoveItem(ItemType itemType) {
        if (--Items[itemType] <= 0) {
            Items.erase(itemType);
        }
    }
};