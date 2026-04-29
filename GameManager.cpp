#include "GameManager.h"
#include "Player.h"

GameManager::~GameManager(){
    delete PLAYER;
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