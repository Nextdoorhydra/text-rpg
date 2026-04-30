#pragma once
#include "Configuration.h"

struct Item {
    ItemType Type;
    std::string Name;
    int Price;

    Item(ItemType type, int price);

    // 사용 비권장
    void PrintInfo() const;

    // map에 넣으려면 비교 연산자 오버로딩 해야함
    bool operator<(const Item& other) const;
};