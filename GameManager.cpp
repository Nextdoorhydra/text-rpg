#include "GameManager.h"
#include "Player.h"
#include "Monster.h"
#include "Goblin.h"
#include "Slime.h"
#include "Dragon.h"
#include "Orc.h"

GameManager::~GameManager(){
    delete PLAYER;
    delete MONSTER;
}

void GameManager::MakePlayer() {
    if (PLAYER) {
        delete PLAYER;
    }
    PLAYER = new NoClassPlayer("Unknown", 999, 999, 999, 999);
}

Player* GameManager::GetPlayer() {
    return PLAYER;
}

Player* GameManager::SetPlayer(Player* newPlayer) {
    Inventory* inv = nullptr;

    if (PLAYER) {
        inv = PLAYER->Inven; // 기존 플레이어의 인벤토리 저장
        delete PLAYER; // 기존 플레이어 메모리 해제
    }

    newPlayer->Inven = inv; // 새 플레이어에 기존 인벤토리 할당
    PLAYER = newPlayer; // 새 플레이어 설정

    return PLAYER;
}

Monster* GameManager::GetMonster() {
    return MONSTER;
}

Monster* GameManager::SpawnMonster() {

    if (MONSTER) {
        delete MONSTER; // 기존 플레이어 메모리 해제
    }

    switch (PROGRESSION) {
    case 0:
        MONSTER = new Slime(std::string("Slime"), 50, 10, 1, 1, Item(ItemType::SlimeJelly, 1));
        break;
    case 1:
        MONSTER = new Goblin(std::string("Goblin"), 100, 21, 3, 1, Item(ItemType::GoblinEar, 3));
        break;
    case 2:
        MONSTER = new Orc(std::string("Orc"), 200, 51, 5, 3, Item(ItemType::OrcHead, 5));
        break;
    default:
        MONSTER = new Dragon(std::string("Dragon"), 1000, 100, 20, 10, Item(ItemType::DragonHorn, 10));
        break;
    }

    return MONSTER;
}