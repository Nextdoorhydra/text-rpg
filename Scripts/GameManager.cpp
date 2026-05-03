#include "GameManager.h"
#include "Player.h"
#include "DungeonRoom.h"
#include "Alchemyworkshop.h"
#include "Item.h"
#include "IGameState.h"

GameManager::GameManager() {
    ALCHEMY_WORKSHOP = new Alchemyworkshop();

    for (int i = 0; i < ROOM_SIZE; ++i) {
        ROOM[i] = new DungeonRoom(i);
    }
}

GameManager::~GameManager() {
    delete CURRENT_STATE;
    CURRENT_STATE = nullptr;
    delete PLAYER;
    PLAYER = nullptr;
    delete MONSTER;
    MONSTER = nullptr;
    delete ALCHEMY_WORKSHOP;
    ALCHEMY_WORKSHOP = nullptr;

    for (int i = 0; i < ROOM_SIZE; ++i) {
        if (ROOM[i]) {
            delete ROOM[i];
            ROOM[i] = nullptr;
        }
    }
}

void GameManager::SetCurrentState(IGameState* newState) {
    if (CURRENT_STATE != nullptr) {
        delete CURRENT_STATE;
    }
    CURRENT_STATE = newState;
}

IGameState* GameManager::GetCurrentState() {
    return CURRENT_STATE;
}

Alchemyworkshop* GameManager::GetAlchemyworkshop() {
    return ALCHEMY_WORKSHOP;
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
    Inventory<Item>* inv = nullptr;

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
        delete MONSTER; // 기존 몬스터 메모리 해제
        MONSTER = nullptr; // Dangling Pointer 방지
    }

    MONSTER = ROOM[PROGRESSION]->SpawnRoomMonster();

    return MONSTER;
}

int GameManager::GetProgression() const {
    return PROGRESSION;
}

void GameManager::AdvanceProgression() {
    if (PROGRESSION < ROOM_SIZE - 1) {
        PROGRESSION++;
    }
}

bool GameManager::TrySetProgression(int k) {
    if (k > ROOM_SIZE - 1 || k < 0) {
        return false; // 유효하지 않은 방 번호면 실패 반환
    }

    PROGRESSION = k;
    return true;  // 성공적으로 세팅됨
}