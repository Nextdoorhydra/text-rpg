#pragma once
#include <map>
#include "Item.h"

class Inventory {
public:
    std::map<Item, int> Items;


    bool TryUseItem(Item item) {
        if (Items.count(item) > 0 && Items[item] > 0) {

            RemoveItem(item);
            return true;
        }

        return false;
    }

    // 아이템 추가
    void AddItem(Item item) {
        Items[item]++;
    }
    // 아이템 제거
    void RemoveItem(Item item) {
        if (--Items[item] <= 0) {
            Items.erase(item);
        }
    }
};