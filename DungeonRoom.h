#pragma once
#include "Monster.h"
#include "Goblin.h"
#include "Slime.h"
#include "Dragon.h"
#include "Orc.h"
#include "Item.h"

class DungeonRoom {
private:
    int RoomLevel; // 방의 단계 (0~3)

public:
    DungeonRoom(int level) : RoomLevel(level) {}
    ~DungeonRoom() {}

    // 이 방에 할당된 고정 몬스터를 생성해서 반환하는 팩토리 메서드
    Monster* SpawnRoomMonster() {
        switch (RoomLevel) {
        case 0:
            return new Slime(std::string("Slime"), 50, 10, 1, 1, Item(ItemType::SlimeJelly, 1));
        case 1:
            return new Goblin(std::string("Goblin"), 100, 21, 3, 1, Item(ItemType::GoblinEar, 3));
        case 2:
            return new Orc(std::string("Orc"), 200, 51, 5, 3, Item(ItemType::OrcHead, 5));
        default:
            return new Dragon(std::string("Dragon"), 1000, 100, 20, 10, Item(ItemType::DragonHorn, 10));
        }
    }
};